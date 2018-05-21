
#include <iostream>
#include "SDL.h"

using std::cout;
using std::endl;
using std::string;

#define WORM_WALK "worm_walk.bmp"


#define HIGH 1366
#define WITH 768
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
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
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
        this->position = position;
        this->position.x = x;
        this->position.y = y;
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
        if(this->step == 15){
            in_movement = false;
            this->step = 0;
        }
        else if(this->step == 14){
            move_left(10);
        }
        next_internal_mov();
        this->step +=1;
        
        
    }




};

/////////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char *args[]){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *screen;
    SDL_Event event;
    
    SDL_Init(SDL_INIT_VIDEO); // init video
   
    //default high and with
    int w = WITH;
    int h = HIGH;

    SDL_DisplayMode dm;
    //getting the actual size of screen computer
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0){
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    } else {
        w = dm.w;
        h = dm.h;
    }

    //creamos la pantalla
    window = SDL_CreateWindow("WORRRRMSSS", 0, 0, w,h-UP_BORDER_HIGH, 0);
   

    //tomamos la superficie de la pantalla
    screen = SDL_GetWindowSurface(window);

    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;


    Color colorkey_beam(0,255,0);

    Picture beam("viga.bmp", colorkey_beam,1,2);
    int position_beam1_x = w/2-100;
    int position_beam1_y = h/2+40;
    beam.draw(screen,position_beam1_x,position_beam1_y);


    Picture beam2("viga.bmp", colorkey_beam,1,2);
    int position_beam2_x = 50;
    int position_beam2_y = 50;
    beam2.draw(screen, position_beam2_x, position_beam2_y);


    Color colorkey(128,128,192);
    Animation personaje(WORM_WALK,colorkey,1,15,w/2,h/2,100);
    personaje.draw(screen);

 


    while(true){

        //control de evento para cerrar ventana
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        else if(event.type == SDL_KEYDOWN){
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_LEFT] ){
                state = NULL;
                personaje.wish_to_move();
            } 
        }

        //actualiza el dibujo de la superficie en la pantalla
        SDL_UpdateWindowSurface(window);

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

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

