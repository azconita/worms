
#include <iostream>
#include "SDL.h"

using std::cout;
using std::endl;
using std::string;

#define WORM_WALK "worm_walk.bmp"


#define HIGH 1366
#define WITH 768

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



void draw(SDL_Renderer *renderer, SDL_Rect position){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, this->surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return;
    }

    SDL_Rect dimention = get_dimention();
    position.h = dimention.h;
    position.w = dimention.w;

    SDL_RenderCopy(renderer, texture, &dimention,&position);
    SDL_DestroyTexture(texture);
    

}


void next_internal_mov(){
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

/////////////////////////////////////////////////////////////////////////////////////////


};

class Animation {
    Uint32 timer;
    Picture picture;
    SDL_Rect dimention;
    SDL_Rect position;
public:
    Animation(const char * bmp_path, Color color,int columns, int rows,SDL_Rect position, Uint32 timer): 
    picture(bmp_path, color, columns, rows ){
        this->timer = timer;
        this->position = position;
    }

    bool is_time_to_move(Uint32 time_passed){
        return (time_passed > this->timer);

    }


void draw(SDL_Renderer *renderer){
    this->picture.draw(renderer, this->position);

}

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
    this->picture.next_internal_mov();
}

};

/////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *args[]){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;

  
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_DisplayMode dm;

    //default high and with
    int w = WITH;
    int h = HIGH;

    //getting the actual size of screen computer
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0){
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    } else {
        w = dm.w;
        h = dm.h;
    }

    if (SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1, time;


    SDL_Rect position_beam1,position_beam2;

        Color colorkey_beam(0,255,0);

        SDL_Rect position2;

        position_beam1.x = w/2-20;
        position_beam1.y = h/2-20;

        Picture beam("viga.bmp", colorkey_beam,1,2);
        beam.draw(renderer,position_beam1);

        position_beam2.x = 50;
        position_beam2.y = 50;

        Picture beam2("viga.bmp", colorkey_beam,1,2);
        beam2.draw(renderer,position_beam2);

        SDL_Rect position_worm;
        position_worm.x = w/2;
        position_worm.y = h/2;
        Color colorkey(128,128,192);
        Animation personaje(WORM_WALK,colorkey,1,15,position_worm,100);


        
    int step = 0;

    while (true) {

        //control de evento para cerrar ventana
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }

        // Referencia de tiempo
        t1 = SDL_GetTicks();
        time = t1 -t0;

       
        if(personaje.is_time_to_move(time)) {
            step += 1;
            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();
            personaje.next_internal_mov();
            personaje.move_left(1);

            if(step == 15){
                step = 0;
                personaje.move_left(15);
            }
            // Movimiento del personaje
            personaje.draw(renderer);

           SDL_RenderPresent(renderer);


        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

