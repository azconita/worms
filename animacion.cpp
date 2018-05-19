
#include "SDL.h"

class Color{
public:
    int r,g,b;
    Color(int r,int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }

};

class Animation {
    Uint32 timer;
    SDL_Surface *surface;
    SDL_Rect dimention;
    SDL_Rect position;
public:
    Animation(const char * bmp_path, Color color, SDL_Rect dimention,SDL_Rect position, Uint32 timer){
        this->timer = timer;
        this->dimention = dimention;
        this->position = position;
        this->surface = SDL_LoadBMP(bmp_path);
        if (!this->surface) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
            return;
        }

        Uint32 colorkey = SDL_MapRGB(surface->format, color.r, color.g, color.b);
        SDL_SetColorKey(this->surface, SDL_TRUE, colorkey);
    }

    bool is_time_to_move(Uint32 time_passed){
        return (time_passed > this->timer);

    }


void draw(SDL_Renderer *renderer){
    SDL_Texture *texture = NULL;
    texture = SDL_CreateTextureFromSurface(renderer, this->surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &this->dimention,&this->position);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);

}

void move_left(){
    this->position.x -=1;

}

void move_right(){
    this->position.x +=1;

}

void move_up(){
    his->position.y +=1;

}

void move_down(){
    his->position.y -=1;

}



/*~Animation(){
    SDL_FreeSurface(this->surface);
}*/

};



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
    int w = 1366;
    int h = 768;

    //getting the actual size of screen computer
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0){
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    } else {
        w = dm.w-50;
        h = dm.h-50;
    }
    

    if (SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;


    int i = w/2;

    SDL_Rect dimention;
    SDL_Rect position;

        dimention.x = 0;
        dimention.y = 0;
        dimention.w = 900;   
        dimention.h = 650;

        position.x = i;
        position.y = h/2;
        position.w = 70;
        position.h = 70;

       

        Color colorkey(255,255,255);
        Animation gusano("gusanito1.bmp",colorkey,dimention,position,100);



    while (true) {

        //control de evento para cerrar ventana
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
  

        // Referencia de tiempo
        t1 = SDL_GetTicks();

        if(gusano.is_time_to_move(t1 - t0)) {
            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();

            gusano.move_left();
            // Movimiento del personaje
            gusano.draw(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}