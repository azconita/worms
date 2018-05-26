#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "Client_Constants.h"
#include "resources_definitions.h"

#include "stage.h"
#include "Beam.h"
#include "Worm.h"


//g++ -std=c++11 main.cpp stage.cpp Beam.cpp Worm.cpp Constants.cpp Weapon.cpp Bazooka.cpp -lBox2D -lSDL -lSDL_image -lyaml-cpp -lSDL_ttf  -g


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
#define BAZOOKA 3



////////////////////////////////////////77
enum State{
   Still,
   Walk,
   Fall,
   Jump,
   Worm_missile
};

enum Weapon_name{
    Bazooka,
    Mortar
};

enum Direction{
    Right,
    Left
};

enum Color_Name{
    White,
    Orange,
    Green,
    Purple,
    Pink,
    Yellow,
    Red,
    Blue
};




/////////////////////////////////////////////////////////////////////////////////////////
class Color{
public:
    int r,g,b;
    Color(int r,int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }

    static Color create(Color_Name color_name){
        switch(color_name){
            case(White):{
                Color white(255,255,255);
                return white;
            }
            case(Orange):{
                Color orange(244, 95, 66);
                return orange;
            }
            case(Yellow):{
                Color yellow(229, 220, 57);
                return yellow;
            }
            case(Green):{
                Color green(57, 229, 114);
                return green;
            }
            case Blue:{
                Color blue(83, 144, 237);
                return blue;
            }
            case Purple:{
                Color purple(166, 109, 219);
                return purple;
            }
            case Pink:{
                Color pink(239, 115, 198);
                return pink;
            }
            case Red:{
                Color red(237, 78, 102);
                return red;
            }
        }
        Color black(0,0,0);
        return black;

    }

};

/////////////////////////////////////////////////////////////////////////////////////////
class Picture{
    int rows, columns;
    int row_num, column_num;
    int w, h;

    Direction default_direction;
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
        this->default_direction = Left; //todas las fotos estan para la izquierda


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
int get_height(){
    return this->h;
}

int get_width(){
    return this->w;
}

void draw(SDL_Surface *screen, int x, int y){
    SDL_Rect position;
    position.x = x;
    position.y = y;
    draw(screen, position, this->default_direction);
}



void draw(SDL_Surface *screen, SDL_Rect position, Direction direction){
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


void next_sprite_figure(Direction direction){
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

void previous_sprite_figure(Direction direction){
    if(this->default_direction != direction){ //hay que usar la inversa
        this->column_num +=1; // paso al siguiente en la misma fila
        if(this->column_num >= this->columns){ //si ya no hay mas
            this->column_num = 0; //paso al primero
            this->row_num -= 1; //pero de la fila de arriba
            if(this->row_num < 0){
                this->row_num = this->rows - 1; //si estaba en el ultimo vuelvo a empezar
            
            }
        }
        return;
    }

    this->column_num -=1; // paso al anterior en la misma fila
    if(this->column_num < 0){ //si ya no hay anterior
        this->column_num = this->columns - 1; //paso al ultomo
        this->row_num -= 1; //pero de la fila de arriba
        if(this->row_num < 0){
            this->row_num = this->rows - 1; //si estaba en el primero vuelvo al ultimo
        }
    }  
}

bool is_in_first_figure(Direction direction){
    if(this->default_direction != direction){
        return (this->row_num <= 0 && this->column_num >= this->columns-1);
    }
    return (this->row_num <= 0 && this->column_num <= 0);
    
}

bool is_in_last_figure(Direction direction){
     if(this->row_num >= this->rows -1 ){
        printf("%i es la ultima\n",this->row_num );
    }
   
    if(this->default_direction != direction){
        return (this->row_num >= this->rows-1 && this->column_num <= 0);
    }
    return (this->row_num >= this->rows-1 && this->column_num >= this->columns-1);

}


/*~Picture(){
    SDL_FreeSurface(this->surface);
}*/

};


/////////////////////////////////////////////////////////////////////////////////////////

class Animation {

    Picture picture;
    SDL_Rect dimention;
    int figures_num;
    int step;
    Direction direction;


public:

    Animation(const char * bmp_path, Color color,int columns, int rows):
    picture(bmp_path, color, columns, rows ){
        this->figures_num = columns * rows;
        this->step = 0;
        this->direction = Left;

    }

    int get_height(){
        return this->picture.get_height();
    }

    int get_width(){
        return this->picture.get_width();
    }


    void set_current_direction(Direction direction){
        this->direction = direction;
    }

    Direction get_current_direction(){
        return this->direction;
    }


    void draw(SDL_Surface *screen, int x, int y){
        SDL_Rect position;
        position.x = x;
        position.y = y;
        this->picture.draw(screen, position, this->direction);
    }

    bool continue_internal_movement(){
        this->picture.next_sprite_figure(this->direction);
        this->step += 1;
        if(this->step == this->figures_num){
            this->step = 0;
            return false;
        }
        return true;
    }

    bool point_up(){
        if(!this->picture.is_in_last_figure(this->direction)){
            this->picture.next_sprite_figure(this->direction);
            return true;
        }
        return false;

    }
    bool point_down(){
        if(!this->picture.is_in_first_figure(this->direction)){
            this->picture.previous_sprite_figure(this->direction);
            return true;
        }
        return false;
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

    static Animation get_worm_missile(){
        Color colorkey(WORM_MISSILE_R,WORM_MISSILE_G,WORM_MISSILE_B);
        Animation worm(WORM_MISSILE,colorkey,WORM_MISSILE_COLUMNS,WORM_MISSILE_ROWS);
        return worm;
    }


};

/////////////////////////////////////////////////////////////////////////////////////////

#define GRADES_PER_STEP 5.8064516129 //180/31


class Worm_Animation_Controller{
    int x, y;
    State state;
    Weapon_name * weapon;
    float grades;
    std::map<int,Animation> animations;
    
public:

Worm_Animation_Controller(int initial_x, int initial_y){
    this->x = initial_x;
    this-> y = initial_y;
    this->state = Still;
    this->weapon = NULL;
    this->grades = -90;
    Animation worm_walk = Animation_Factory::get_worm_walk();

    this->animations.insert(std::pair<int,Animation>(Still,worm_walk));
    this->animations.insert(std::pair<int,Animation>(Walk,worm_walk));

    Animation worm_fall = Animation_Factory::get_worm_fall();
    this->animations.insert(std::pair<int,Animation>(Fall,worm_fall));

    Animation worm_jump = Animation_Factory::get_worm_jump();
    this->animations.insert(std::pair<int,Animation>(Jump,worm_jump));

    Animation worm_missile = Animation_Factory::get_worm_missile();
    this->animations.insert(std::pair<int,Animation>(Worm_missile,worm_missile));
}


bool is_armed(){
    return(this->weapon != NULL);
}

void change_direction(Direction direction){
    for(std::map<int,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){ 
        animation_iter->second.set_current_direction(direction);
    }
    if(this->state == Still){
        change_state(Walk);
    }
    
}

void change_state(State state){
    this->state = state; //sigue en la misma dire que antes
}

void take_weapon(Weapon_name weapon){
    this->weapon = &weapon;
    this->grades = -90;
    State weapon_state = Worm_missile;
    change_state(weapon_state);

}

float point_down_weapon(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state); 
    if(animation_iter->second.point_down()){
        this->grades-=GRADES_PER_STEP; //31 fotos/180 grados
    }
    return this->grades;

}

float point_up_weapon(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state); 
    if(animation_iter->second.point_up()){
       this->grades+=GRADES_PER_STEP;
    }
    return this->grades;
}


Direction get_direction(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state); 
    return animation_iter->second.get_current_direction();
}


void move(int position_x, int position_y){
    if(this->state == Walk && position_y > this->y){ //aumenta el y, se cae
        this->state = Fall;
    }
    this->x = position_x;
    this->y = position_y;
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);

    if(this->state == Jump && this->state ==Walk){
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
                    turn_worm.change_direction(Left);
                    this->stage.make_action(0,LEFT);    
                    break;
                case SDLK_RIGHT:
                    cout << "se apreto derecha " << endl;
                    turn_worm.change_direction(Right);
                    this->stage.make_action(0,RIGHT);
                    break;
                case SDLK_UP:
                    cout << "se apreto arriba " << endl;
                    if(turn_worm.is_armed()){
                        float grades = turn_worm.point_up_weapon();
                        printf("%f\n",grades );
                        //this->stage.make_action(0,grades);
                    }else{
                        turn_worm.change_state(Jump);
                        this->stage.make_action(0,UP);
                    }
                    break;
                case SDLK_DOWN:
                    cout << "se apreto abajo " << endl;
                    if(turn_worm.is_armed()){
                        float grades = turn_worm.point_down_weapon();
                        printf("%f\n",grades );
                        //this->stage.make_action(0,grades);
                    }
                    break;
                case SDLK_b:
                    cout << "se apreto b -> bazooka" << endl;
                    turn_worm.take_weapon(Bazooka);
                    //this->stage.make_action(0,Bazooka);
                case SDLK_m:
                    cout << "se apreto M -> Mortar" << endl;
                    turn_worm.take_weapon(Mortar);
                    //this->stage.make_action(0,Mortar);
                }
                break;
    }
    return true;
    }

};


/////////////////////////////////////////////////////////////////////////////////////////////
class Graphic_Designer{
    TTF_Font *font;
    SDL_Color text_color;



public:

Graphic_Designer(int i){
    // Initialize SDL_ttf library
    if (TTF_Init() != 0) {
      cout << "TTF_Init() Failed: " << TTF_GetError() << endl;
      SDL_Quit();
      exit(1);
    }


    this->font = TTF_OpenFont("resources/Amiko-Bold.ttf", 10);
    if (this->font == NULL){
        cout << "TTF_OpenFont() Fail: " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    this->text_color =  {0, 0, 0};


}



void show_circle_life(SDL_Surface * screen, int life, int x, int y, Color color){

    char str_life[10];
    if(life < 100){
        sprintf(str_life, " %d ", life);
    }else{
        sprintf(str_life, "%d", life);
    }


    SDL_Surface * text = TTF_RenderText_Solid(font,str_life,text_color);
    if (text == NULL){
        cout << "TTF_RenderText_Solid(): " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    SDL_Rect rectangle;
    rectangle.x = x-2;
    rectangle.y = y-2;
    rectangle.h = text->h + 2;
    rectangle.w = text->w + 4 ;
    Uint32 colorkey = SDL_MapRGBA(screen->format, color.r, color.g, color.b,0.2);
    SDL_FillRect(screen, &rectangle, colorkey);

    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = text->h;
    dimention.w = text->w;

    SDL_Rect position;
    position.x = x;
    position.y = y;
    position.h = text->h;
    position.w = text->w;
    SDL_BlitSurface(text, &dimention, screen, &position);


}


};


//////////////////////////////////////////////////////////////////////////////////////////


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

    Uint32 colorkey = SDL_MapRGBA(screen->format, 0, 255, 0,0.5);
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
/////////////////////////////////////////////////////////////////////////////

class Water_animation_controller{
    Color colorkey;
    Animation water_left;
    Animation water_right;
    int screen_height;
    int show_counter;
    int speed;

public:
    Water_animation_controller(int screen_height,int speed):
        colorkey(WATER_R,WATER_G,WATER_B),
        water_left(WATER, this->colorkey,WATER_COLUMNS,WATER_ROWS),
        water_right(WATER, this->colorkey,WATER_COLUMNS,WATER_ROWS)
    {
        this->speed = speed;
        this->show_counter = 0;
        this->screen_height = screen_height - 50;
        this->water_right = water_right;
        this->water_left = water_left;

    }

    void show(SDL_Surface * screen){
        if(this->show_counter == this->speed){
            this->show_counter = 0;
            water_left.continue_internal_movement();
            water_right.continue_internal_movement();

        }else{
            this->show_counter +=1;
        }
        water_left.draw(screen,0,this->screen_height-water_left.get_height());
        water_right.draw(screen,water_left.get_width()-2,this->screen_height-water_right.get_height());

    }


};

//////////////////////////////////////////////////////////////////////////////////

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




void show_worms(StageDTO s, SDL_Surface *screen, std::map<int,Worm_Animation_Controller> & worms, Graphic_Designer & graphic_designer){

    for (auto w: s.worms) {

        std::vector<std::tuple<float, float>> vertices = w.second;

        //debug_box2d_figure(screen, vertices);

        std::tuple<float, float> up_left_vertex = vertices[0];
        int up_left_vertex_x = get_pixels(std::get<0>(up_left_vertex));
        int up_left_vertex_y = get_pixels(std::get<1>(up_left_vertex));

        std::map<int,Worm_Animation_Controller>::iterator worms_iter = worms.find(w.first);
        worms_iter->second.move(up_left_vertex_x, up_left_vertex_y);
        worms_iter->second.show(screen);

        Color player_color = Color::create(Orange);
        int initial_life = 100;
        graphic_designer.show_circle_life(screen, initial_life,up_left_vertex_x+20,up_left_vertex_y-5,player_color);

    }
}




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
       cout << "Modo video no soportado: " << SDL_GetError() << endl;
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

    // Set the title bar
    SDL_WM_SetCaption("Worms game", "Worms");

    Graphic_Designer graphic_designer(1);


    //------------------------------------

    Stage stage("stage1");

    StageDTO s = stage.get_stageDTO();

    Water_animation_controller water(screen_height, 3);

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
            //dibujo las vigas y el agua
            show_beams(s, screen);

            water.show(screen);
            //dibujo los gusanos
            show_worms(s, screen, worms, graphic_designer);

        }

    }

    return 0;
}
