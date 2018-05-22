
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "resources_definitions.h"


using std::cout;
using std::endl;
using std::string;


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
        this->surface = SDL_LoadBMP(bmp_path);
        if (!this->surface) {
            cout <<"Couldn't create surface from image:" << bmp_path << SDL_GetError() << endl; 
            return;
        }

        Uint32 colorkey = SDL_MapRGB(surface->format, color.r, color.g, color.b);
        SDL_SetColorKey(this->surface, SDL_TRUE, colorkey);


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

public:
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

    void move(){
        if(this->step == this->figures_num){
            in_movement = false;
            this->step = 0;
        }
        else if(this->step == this->figures_num - 1){
            move_left(10);
        }
        next_internal_mov();
        this->step +=1;
        
        
    }




};

/////////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char *args[]){

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
        exit(1); 
    }

    atexit(SDL_Quit);
    int screenWidth = SCREEN_DEFAULT_WITH;
    int screenHeight = SCREEN_DEFAULT_HIGH;

    const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();   
    screenWidth = info->current_w;
    screenHeight = info->current_h;

    
   
    if(SDL_VideoModeOK(screenWidth, screenHeight, 24, SDL_HWSURFACE|SDL_DOUBLEBUF) == 0) {
        // Comprobamos que sea compatible el modo de video
       cout << "Modo no soportado: " << SDL_GetError() << endl;
       exit(1);
    }

    // Establecemos el modo de video
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(screenWidth, screenHeight, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(screen == NULL) {
        cout << "No se pudo establecer el modo de video: "
        << SDL_GetError() << endl;
        exit(1);
    }
    

    Color colorkey_beam(BIG_BEAM_R,BIG_BEAM_G,BIG_BEAM_B);

    Picture beam(BIG_BEAM, colorkey_beam,BIG_BEAM_COLUMNS,BIG_BEAM_ROWS);
    int position_beam1_x = screenWidth/2-100;
    int position_beam1_y = screenHeight/2+40;
    beam.draw(screen,position_beam1_x,position_beam1_y);


    Picture beam2(BIG_BEAM, colorkey_beam,BIG_BEAM_COLUMNS,BIG_BEAM_ROWS);
    int position_beam2_x = 50;
    int position_beam2_y = 50;
    beam2.draw(screen, position_beam2_x, position_beam2_y);


    Color colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);
    Animation personaje(WORM_WALK,colorkey,WORM_WALK_COLUMNS,WORM_WALK_ROWS,screenWidth/2,screenHeight/2,100);
    personaje.draw(screen);

 

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
                        personaje.wish_to_move();
                        break;
                    }
                    break;
        }

        //actualiza el dibujo de la superficie en la pantalla
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();
       
        if(personaje.is_time_to_move(t1 -t0)) {
                
            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();

            //borro todo lo que estaba
            //toda la pantalla en negro
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
            
            //tengo que volver a dibujar todo
            beam.draw(screen,position_beam1_x,position_beam1_y);
            beam2.draw(screen,position_beam2_x, position_beam2_y);
            
            // Movimiento del personaje
            personaje.move();
            personaje.draw(screen);
        }
        
    } 

    return 0;
}

