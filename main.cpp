#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "resources_definitions.h"
#include "stage.h"
#include "Beam.h"
#include "Worm.h"

//g++ stage.cpp Beam.cpp Worm.cpp main.cpp -g  -std=c++11  -lBox2D -lSDL -lSDL_image




using std::cout;
using std::endl;
using std::string;
using std::map;


#define SCREEN_DEFAULT_WITH 1366
#define SCREEN_DEFAULT_HIGH 768

#define UP_BORDER_HIGH 50

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

    SDL_Surface *surface;

    SDL_Rect get_dimention(){
    SDL_Rect dimention;
    // Separaciones de 2 píxeles dentro de las rejillas para observar
    // bien donde empieza una imagen y donde termina la otra
    dimention.w = this->w - 2;
    dimention.h = this->h - 2;

    // Cálculo de la posición de la imagen // dentro de la rejilla
    dimention.x = (this->row_num * this->w) + 2;
    dimention.y = (this->column_num * this->h) + 2;

    return dimention;

}




public:
    Picture(const char * bmp_path, Color color, int columns, int rows){
        this->rows = rows;
        this->columns = columns;
        this->row_num = 0;
        this->column_num = 0;
        //this->surface =     IMG_Load(bmp_path);
        SDL_Surface *tmp = SDL_LoadBMP(bmp_path);
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

        // El ancho de una imagen es el total entre el número de columnas
        this->w = surface->w / columns;
        // El ato de una imagen es el total entre el número de filas
        this->h = surface->h / rows;

    }
void draw(SDL_Surface *screen, int x, int y){
    SDL_Rect position;
    position.x = x;
    position.y = y;
    draw(screen, position);

}



void draw(SDL_Surface *screen, SDL_Rect position){
    SDL_Rect dimention = get_dimention();

    position.h = dimention.h;
    position.w = dimention.w;

    //printf("x = %i, y = %i, h = %i, w = %i \n",position.x, position.y, position.h, position.w );

    SDL_BlitSurface(this->surface, &dimention, screen, &position);

}


void next_sprite_figure(){
    this->row_num += 1;
    if(this->row_num >= this->columns){
        this->row_num = 0;
        this->column_num +=1;
        if(this->column_num >= this->rows){
            this->column_num = 0;
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
public:
    Uint32 timer;
    int step;
    Picture picture;
    SDL_Rect dimention;
    SDL_Rect position;
    int figures_num;
    bool in_movement;

    void move_left(int step){
        this->position.x -=step;
    }
    void move_right(int step){
        this->position.x +=step;
    }
    void move_up(int step){
        this->position.y +=step;
    }
    void move_down(int step){
        this->position.y -=step;
    }
    int next_internal_mov(){
        this->picture.next_sprite_figure();
    }





    Animation(const char * bmp_path, Color color,int columns, int rows,int x, int y, Uint32 timer):
    picture(bmp_path, color, columns, rows ){
        this->step = 0;
        this->timer = timer;
        this->position.x = x;
        this->position.y = y;
        this->figures_num = columns * rows;
        this->in_movement = false;

    }

    void draw(SDL_Surface *screen){
      this->picture.draw(screen, this->position);
    }

    bool wish_to_move(){
        this->in_movement = true;
    }

    bool is_time_to_move(Uint32 time_passed){
        return ((time_passed > this->timer) && this->in_movement);
    }

    void move(int position_x, int position_y){
        printf("step = %i : x = %i, y = %i \n",this->step, this->position.x,this->position.y );

        if(this->in_movement){
            this->position.y = position_y;
            //si se quiso mover cambia las figuras y despues se mueve
            next_internal_mov();
            this->step +=1;
            if(this->step == this->figures_num){
                this->position.x = position_x;
                this->in_movement = false;
                this->step = 0;
            }
        } else{
            //si se cae por ejemplo
            this->position.x = position_x;
            this->position.y = position_y;

        }

    }

};

/////////////////////////////////////////////////////////////////////////////////////////

float get_x_pixels(float meter_position){
    return  23.5*meter_position+ 500;
}

float get_y_pixels(float meter_position){
    return  23.5*meter_position;
}

void show_beams(StageDTO s, SDL_Surface *screen){

    Color colorkey_beam(BIG_BEAM_R,BIG_BEAM_G,BIG_BEAM_B);

    for (auto b: s.beams) {
        //cout <<"viga " << b.first << endl;
        std::vector<std::tuple<float, float>> positions = b.second;
        std::tuple<float, float> pos = positions[3];
        Picture beam(BIG_BEAM, colorkey_beam,BIG_BEAM_COLUMNS,BIG_BEAM_ROWS);
        int position_beam_x = get_x_pixels(std::get<0>(pos));
        int position_beam_y = get_y_pixels(std::get<1>(pos));
        beam.draw(screen,position_beam_x,position_beam_y);

    }
}

std::map<int,Animation> create_worms(StageDTO s, SDL_Surface *screen){

    std::map<int,Animation> worms;

        Color colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);

    for (auto w: s.worms) {
        int id = w.first;
        std::vector<std::tuple<float, float>> positions = w.second;

        std::tuple<float, float> pos = positions[3];
        int position_worm_x = get_x_pixels(std::get<0>(pos));
        int position_worm_y = get_y_pixels(std::get<1>(pos));

        //creo el gusano y lo gardo en el vector
        Animation worm(WORM_WALK,colorkey,WORM_WALK_COLUMNS,WORM_WALK_ROWS,position_worm_x,position_worm_y,100);
        worms.insert ( std::pair<int,Animation>(id,worm) );
        worm.draw(screen);
    }

    return worms;
}


void show_worms(StageDTO s, SDL_Surface *screen, std::map<int,Animation> & worms){

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
    std::map<int,Animation> worms = create_worms(s, screen);
    printf(" size = %li\n", worms.size() );


    //------------------------------------


    //turno harcodeado
    std::map<int,Animation>::iterator turn_worm_iter = worms.find(0);


    SDL_Event event;

    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;

    bool running=true;
    while(running ){

        //control de evento para cerrar ventana
        SDL_PollEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                cout << "se apreto x -> fin" << endl;
                running = false;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        running=false;
                        break;
                    case SDLK_LEFT:
                        cout << "se apreto izquierda " << endl;
                        turn_worm_iter->second.wish_to_move();
                        stage.make_action(0, 1);
                        break;
                    }
                    break;
        }

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
            //show_worms(s, screen, worms);

            for (auto w: s.worms) {
                std::vector<std::tuple<float, float>> positions = w.second;

                std::tuple<float, float> pos = positions[3];
                int position_worm_x = get_x_pixels(std::get<0>(pos));
                int position_worm_y = get_y_pixels(std::get<1>(pos));


                std::map<int,Animation>::iterator animation_iter = worms.find(w.first);
                //Animation worm = std::move(it->second);

                animation_iter->second.move(position_worm_x,position_worm_y);

                animation_iter->second.draw(screen);
            }

        }

    }

    return 0;
}
