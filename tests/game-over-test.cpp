#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define BUTTON_H 50
#define BUTTON_W 200

class Button{
    SDL_Surface *surface;
    SDL_Rect shadow;
    SDL_Rect box;
    Uint32 black;
    Uint32 gray;
    bool clicked;

public:
    Button(SDL_Surface *surface, Sint16 x, Sint16 y){
        this->surface = surface;
         this->black = SDL_MapRGB(surface->format, 0, 0, 0);
        this->gray = SDL_MapRGB(surface->format, 55, 55, 55);
        this->shadow = {x,y,BUTTON_W,BUTTON_H};
        this->box= {x,y,BUTTON_W-5,BUTTON_H-5};
        this->clicked = false;


    }

    //Handles events and set the button's sprite region
    void  handle_events(int x, int y){
        if( ( x > this->box.x ) && ( x < this->box.x + this->box.w ) &&
         ( y > this->box.y ) && ( y < this->box.y + this->box.h ) ){
            printf("estaba aca adentro\n");
            this->clicked = true;
        }
    }
    
    //Shows the button on the screen
    void show(){
        SDL_FillRect(this->surface, &this->shadow, this->black);
        if(this->clicked == false){
            printf("no se clickeo\n");
            SDL_FillRect(this->surface, &this->box, this->gray);
        }
        
    }

};

bool event_controller(SDL_Event &event, bool & ya_perdio){
    if(SDL_PollEvent(&event) != 1){
        return true; // no hay nuevos event
    }
    switch(event.type){
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE ){
                return false;
            }
        case SDL_MOUSEBUTTONUP:
            ya_perdio = true;
            break;
    }
    return true;
}


bool event_controller2(SDL_Event &event, Button & button){
    if(SDL_PollEvent(&event) != 1){
        return true; // no hay nuevos event
    }
    switch(event.type){
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE ){
                return false;
            }
        case SDL_MOUSEBUTTONUP:
            int x, y;
            SDL_GetMouseState(&x, &y);
            button.handle_events(x,y);
            break;
    }
    return true;
}


int main(){

	SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    int screen_width;
    int screen_height;

    const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
    screen_width = info->current_w;
    screen_height = info->current_h;

    SDL_VideoModeOK(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);

    // Establecemos el modo de video
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);

    SDL_Surface *background = SDL_LoadBMP("background.bmp");

    printf("imagen de fondo = w:%i h:%i\n", background->w, background->h );


    SDL_Rect rectangle;

    SDL_Event event;
    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;

    bool ya_perdio = false;
    bool running=true;
    while(running ){

        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();


        if((t1 -t0) > 17) {

            if(ya_perdio == false){
                // Nueva referencia de tiempo
                t0 = SDL_GetTicks();

                //borro todo lo que estaba
                //toda la pantalla en negro
                SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
                

            	SDL_BlitSurface(background, NULL, screen, NULL);
            
            	//Show the dot on the screen
            	rectangle.x = 100; 
                rectangle.y = 100; 
                rectangle.h = 55;
                rectangle.w = 25; 
                Uint32 colorkey = SDL_MapRGB(screen->format, 255, 255, 0);
                SDL_FillRect(screen, &rectangle, colorkey);

                
                running = event_controller(event, ya_perdio);


            }else{
               SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
                SDL_Surface* background_transparente = SDL_LoadBMP("background.bmp");
                SDL_SetAlpha(background_transparente , SDL_SRCALPHA, 50);
                SDL_BlitSurface(background_transparente , NULL, screen, NULL);

                Button button(screen, 200, 200);
                running = event_controller2(event, button);
                button.show();
                
            }   

        }

    }
}