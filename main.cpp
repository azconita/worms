#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "Client_Constants.h"
#include "resources_definitions.h"
#include "stage.h"
#include "Beam.h"
#include "Worm.h"


//g++ -std=c++11 main.cpp stage.cpp Beam.cpp Worm.cpp Constants.cpp Weapon.cpp Bazooka.cpp -lBox2D -lSDL -lSDL_image -lyaml-cpp -g


using std::cout;
using std::endl;
using std::string;
using std::map;


#define SCREEN_DEFAULT_WITH 1366
#define SCREEN_DEFAULT_HIGH 768

#define UP_BORDER_HIGH 50

#define RIGHT 0
#define LEFT 1
#define UP 2


/////////////////////////////////////////////////////////////////////////////////////////
class Color{
public:
    int r,g,b;
    Color(int r,int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }

};

/////////////////////////////////////////////////////////////////////////////////////////
class Picture{
    int rows, columns;
    int row_num, column_num;
    int w, h;

    int default_direction;
    SDL_Surface *surface;
    SDL_Surface *flipped;

SDL_Rect get_dimention(){
    SDL_Rect dimention;
    // Separaciones de 2 píxeles dentro de las rejillas para observar
    // bien donde empieza una original y donde termina la otra
    dimention.w = this->w - 2;
    dimention.h = this->h - 2;
 
    // Cálculo de la posición de la original // dentro de la rejilla
    
    dimention.x = (this->column_num * this->w) + 2;
    dimention.y = (this->row_num * this->h) + 2; 

    return dimention;
}


SDL_Surface * flip(SDL_Surface * original, Uint32 colorkey){
        SDL_Rect origen;
        SDL_Rect destino;

        // Origen -> ancho una línea
        // Comienzo de copia por el principio
        origen.x = 0;
        origen.y = 0;
        origen.w = 1;
        origen.h = original->h;
        // Destino -> ancho una lína
        // Comienzo de 'pegado' por el final
        // Para lograr la inversión
        destino.x = original->w;
        destino.y = 0;
        destino.w = 1;
        destino.h = original->h;
        SDL_Surface *flipped;
        // Pasamos la imagen a formato de pantalla
        flipped = SDL_DisplayFormat(original);
        if(flipped == NULL) {
            cout << "No podemos convertir la imagen al formato de pantalla" << endl;
            return NULL;
        }
       
        // Preparamos el rectángulo nuevo vacío del color transparente
        SDL_FillRect(flipped, NULL, colorkey);
        // Copiamos linea vertical a linea vertical, inversamente
        for(int i = 0; i < original->w; i++) {
            SDL_BlitSurface(original, &origen, flipped, &destino);
            origen.x = origen.x + 1; destino.x = destino.x - 1;
       }
       return flipped;
} 





public:
    Picture(const char * bmp_path, Color color, int columns, int rows){
        this->rows = rows;
        this->columns = columns;
        this->row_num = 0;
        this->column_num = 0;
        this->default_direction = LEFT; //todas las fotos estan para la izquierda


        SDL_Surface *tmp = IMG_Load(bmp_path);
        //SDL_Surface *tmp = SDL_LoadBMP(bmp_path);
        if (!tmp) {
            cout <<"Couldn't create surface from image:" << bmp_path << SDL_GetError() << endl;
            return;
        }

        this->surface = SDL_DisplayFormat(tmp);
        SDL_FreeSurface(tmp);

        if(this->surface == NULL) {
            cout << "Error: " << SDL_GetError() << endl; exit(1);
        }

        // Calculamos el color transparente, en nuestro caso el verde
         Uint32 colorkey = SDL_MapRGB(this->surface->format, color.r, color.g, color.b);
        // Lo establecemos como color transparente
        SDL_SetColorKey(this->surface, SDL_SRCCOLORKEY, colorkey);

        this->flipped = flip(surface, colorkey);

        // El ancho de una original es el total entre el número de columnas   
        this->w = surface->w / columns;
        // El alto de una original es el total entre el número de filas
        this->h = surface->h / rows;
}


void draw(SDL_Surface *screen, int x, int y){
    SDL_Rect position;
    position.x = x;
    position.y = y;
    draw(screen, position, this->default_direction);
}



void draw(SDL_Surface *screen, SDL_Rect position, int direction){
    SDL_Rect dimention = get_dimention();

    position.h = dimention.h;
    position.w = dimention.w;

    //printf("x = %i, y = %i, h = %i, w = %i \n",position.x, position.y, position.h, position.w );
    
    if(this->default_direction != direction){
        SDL_BlitSurface(this->flipped, &dimention, screen, &position);
        return;
    }

    SDL_BlitSurface(this->surface, &dimention, screen, &position);

}


void next_sprite_figure(int direction){
    if(this->default_direction != direction){ //hay que usar la inversa
        this->column_num -=1; // paso al anterior en la misma fila
        if(this->column_num <= 0){ //si estoy en el primero
            this->column_num = this->columns - 1; //paso al ultimo
            this->row_num += 1; //pero de la fila de abajo
            if(this->row_num >= this->rows){
                this->row_num = 0; //si estaba en el ultimo vuelvo a empezar
            
            }
        }
        return;
    }

    this->column_num +=1; // paso al siguiente en la misma fila
    if(this->column_num >= this->columns){ //si estoy en el ultimo de la fila
        this->column_num = 0; //paso al primero
        this->row_num += 1; //pero de la fila de abajo
        if(this->row_num >= this->rows){
            this->row_num = 0; //si estaba en el ultimo vuelvo a empezar
        }
    }  
}


/*~Picture(){
    SDL_FreeSurface(this->surface);
}*/

};

/////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////

class Animation {

    Picture picture;
    SDL_Rect dimention;
    int figures_num;
    int step;
    int direction;


    int next_internal_mov(){
        this->picture.next_sprite_figure(this->direction);
    }

public:

    Animation(const char * bmp_path, Color color,int columns, int rows):
    picture(bmp_path, color, columns, rows ){
        this->figures_num = columns * rows;
        this->step = 0;
        this->direction = -1;

    }

    void set_current_direction(int direction){
        this->direction = direction;
    }

    int get_current_direction(){
        return this->direction;
    }


    void draw(SDL_Surface *screen, int x, int y){
        SDL_Rect position;
        position.x = x;
        position.y = y;
        this->picture.draw(screen, position, this->direction);
    }

    bool continue_internal_movement(){
        next_internal_mov();
        this->step += 1;
        if(this->step == this->figures_num){
            this->step = 0;
            return false;
        }
        return true;
    }


};

/////////////////////////////////////////////////////////////////////////////////////////
class Animation_Factory{

private:
    Animation_Factory(){}
public:
    static Animation get_worm_walk(){
        Color colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);
        Animation worm(WORM_WALK,colorkey,WORM_WALK_COLUMNS,WORM_WALK_ROWS);
        return worm;
    }

    static Animation get_worm_jump(){
        Color colorkey(WORM_JUMP_R,WORM_JUMP_G,WORM_JUMP_B);
        Animation worm(WORM_JUMP,colorkey,WORM_JUMP_COLUMNS,WORM_JUMP_ROWS);
        return worm;

    }

    static Animation get_worm_fall(){
        Color colorkey(WORM_FALL_R,WORM_FALL_G,WORM_JUMP_B);
        Animation worm(WORM_FALL,colorkey,WORM_FALL_COLUMNS,WORM_FALL_ROWS);
        return worm;

    }


};

/////////////////////////////////////////////////////////////////////////////////////////
#define STILL 0
#define WALK 1
#define FALL 2
#define JUMP 3

enum State{
   Still,
   Walk,
   Fall,
   Jump
};

class Worm_Animation_Controller{
public:
    int x, y;
    State state;
    std::map<int,Animation> animations;


Worm_Animation_Controller(int initial_x, int initial_y){
    this->x = initial_x;
    this-> y = initial_y;
    this->state = Still;
    Animation worm_walk = Animation_Factory::get_worm_walk();
    
    this->animations.insert(std::pair<int,Animation>(STILL,worm_walk));
    this->animations.insert(std::pair<int,Animation>(WALK,worm_walk));

    Animation worm_fall = Animation_Factory::get_worm_fall();
    this->animations.insert(std::pair<int,Animation>(FALL,worm_fall));

    Animation worm_jump = Animation_Factory::get_worm_jump();
    this->animations.insert(std::pair<int,Animation>(JUMP,worm_jump));

   

}
int get_direction(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state); 
    return animation_iter->second.get_current_direction();
}

void wish_to_move(State state, int direction){
    this->state = state;
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state); 
    animation_iter->second.set_current_direction(direction);
}

void wish_to_move(State state){
    int last_direction = get_direction();
    wish_to_move(state, last_direction);
}

void move(int position_x, int position_y){
    if(this->state == Walk && position_y > this->y){ //aumenta el y, se cae
        this->state = Fall;
    } 
    this->x = position_x;
    this->y = position_y; 
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);

    if(this->state != Still){
        if(!animation_iter->second.continue_internal_movement()){
            this->state = Still;
        }
    } 

}

void show(SDL_Surface * screen){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state); 
    animation_iter->second.draw(screen, this->x, this->y); 
}

};

/////////////////////////////////////////////////////////////////////////////////////////

float get_pixels(float meter_position){
    return  23.5*meter_position;
}

void debug_box2d_figure(SDL_Surface *screen, std::vector<std::tuple<float, float>> vertices){

    int i = 0;
    for(auto vertice: vertices){
        int x = std::get<0>(vertice);
        int y = std::get<1>(vertice);
        //printf("v %i = x:%i, y:%i\n", i,x, y );
        i +=1;
    }


    std::tuple<float, float> up_left_vertex = vertices[0];
    int up_left_vertex_x = std::get<0>(up_left_vertex);
    int up_left_vertex_y = std::get<1>(up_left_vertex);

    std::tuple<float, float> down_right_vertex = vertices[2];
    int down_right_vertex_x = std::get<0>(down_right_vertex);
    int down_right_vertex_y = std::get<1>(down_right_vertex);
        
    //dibujo un rectangulo
    SDL_Rect rectangle;
    rectangle.x = get_pixels(up_left_vertex_x);
    rectangle.y = get_pixels(up_left_vertex_y);
    rectangle.h = get_pixels(down_right_vertex_y - up_left_vertex_y);
    rectangle.w = get_pixels(down_right_vertex_x - up_left_vertex_x);

    //printf("rectangle = x : %i y: %i h: %i w: %i\n",rectangle.x,rectangle.y,rectangle.h,rectangle.w );

    Uint32 colorkey = SDL_MapRGBA(screen->format, 0, 255, 0, 5);
    SDL_FillRect(screen, &rectangle, colorkey);

}


void show_beams(StageDTO s, SDL_Surface *screen){

    Color colorkey_beam(BEAM_R,BEAM_G,BEAM_B);

    for (auto b: s.beams) {
        
        std::vector<std::tuple<float, float>> vertices = b.second;

        //debug_box2d_figure(screen, vertices);

        
        std::tuple<float, float> up_left_vertex = vertices[0];
        int up_left_vertex_x = get_pixels(std::get<0>(up_left_vertex));
        int up_left_vertex_y = get_pixels(std::get<1>(up_left_vertex));
        

        Picture beam(BEAM, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        beam.draw(screen,up_left_vertex_x, up_left_vertex_y);

    }

}

std::map<int,Worm_Animation_Controller> create_worms(StageDTO s, SDL_Surface *screen){

    std::map<int,Worm_Animation_Controller> worms;

    Color colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);

    for (auto w: s.worms) {

        int id = w.first;
        std::vector<std::tuple<float, float>> vertices = w.second;

        std::tuple<float, float> up_left_vertex = vertices[0];
        int position_worm_x = get_pixels(std::get<0>(up_left_vertex));
        int position_worm_y = get_pixels(std::get<1>(up_left_vertex));

        //creo el gusano y lo gardo en el vector
        Worm_Animation_Controller worm(position_worm_x, position_worm_y);
        worms.insert(std::pair<int,Worm_Animation_Controller>(id,worm));
        worm.show(screen);

    }

    return worms;
}




void show_worms(StageDTO s, SDL_Surface *screen, std::map<int,Worm_Animation_Controller> & worms){

    for (auto w: s.worms) {

        std::vector<std::tuple<float, float>> vertices = w.second;

        //debug_box2d_figure(screen, vertices);

        std::tuple<float, float> up_left_vertex = vertices[0];
        int up_left_vertex_x = get_pixels(std::get<0>(up_left_vertex));
        int up_left_vertex_y = get_pixels(std::get<1>(up_left_vertex));

        std::map<int,Worm_Animation_Controller>::iterator worms_iter = worms.find(w.first); 
        worms_iter->second.move(up_left_vertex_x, up_left_vertex_y);
        worms_iter->second.show(screen);

    }
}
//////////////////////////////////////////////////////////////////////////////////////////
class Event_Controller{
    Stage & stage;
    SDL_Event &  event;
public:
    Event_Controller(SDL_Event & event, Stage & stage):
        event(event),
        stage(stage)
         {}

bool continue_running(Worm_Animation_Controller& turn_worm){
    SDL_PollEvent(&this->event);
    switch(event.type){
        case SDL_QUIT:
            cout << "se apreto x -> fin" << endl;
            return false;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    return false;

                case SDLK_LEFT:
                    cout << "se apreto izquierda " << endl;
                    turn_worm.wish_to_move(Walk,LEFT);
                    this->stage.make_action(0,LEFT);    
                    break;
                case SDLK_RIGHT:
                    cout << "se apreto derecha " << endl;
                    turn_worm.wish_to_move(Walk,RIGHT);
                    this->stage.make_action(0,RIGHT);
                    break;
                case SDLK_UP:
                    cout << "se apreto arriba " << endl;
                    turn_worm.wish_to_move(Jump);
                    this->stage.make_action(0,UP);
                    break;
                }
                break;  
    }
    return true;
    }

};


//////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *args[]){

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    atexit(SDL_Quit);
    int screen_width = SCREEN_DEFAULT_WITH;
    int screen_height = SCREEN_DEFAULT_HIGH;

    const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
    screen_width = info->current_w;
    screen_height = info->current_h;



    if(SDL_VideoModeOK(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF) == 0) {
        // Comprobamos que sea compatible el modo de video
       cout << "Modo no soportado: " << SDL_GetError() << endl;
       exit(1);
    }

    // Establecemos el modo de video
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(screen == NULL) {
        cout << "No se pudo establecer el modo de video: "
        << SDL_GetError() << endl;
        exit(1);
    }



    //------------------------------------

    Stage stage("stage1");

    StageDTO s = stage.get_stageDTO();

    //dibujo las vigas
    show_beams(s, screen);

    //dibujo los gusanos en su posicion inicial
    std::map<int,Worm_Animation_Controller> worms = create_worms(s, screen);
    printf(" size = %li\n", worms.size() );


    //------------------------------------


    //turno harcodeado
    std::map<int,Worm_Animation_Controller>::iterator turn_worm_iter = worms.find(0);


    SDL_Event event;
    Event_Controller event_controller(event, stage);

    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;

    bool running=true;
    while(running ){

        running = event_controller.continue_running(turn_worm_iter->second);
        //actualiza el dibujo de la superficie en la pantalla
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();
        //update
        stage.update();
        StageDTO s = stage.get_stageDTO();

        if((t1 -t0) > 100) {
            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();

            //borro todo lo que estaba
            //toda la pantalla en negro
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
            //dibujo las vigas 
            show_beams(s, screen); 
            //dibujo los gusanos
            show_worms(s, screen, worms);           
        }

    }

    return 0;
}
