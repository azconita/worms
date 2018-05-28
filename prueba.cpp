#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>



//g++ -std=c++11 prueba.cpp -lBox2D -lSDL -lSDL_image -lyaml-cpp -lSDL_ttf  -lSDL_gfx -g


using std::cout;
using std::endl;
using std::string;



#define SCREEN_DEFAULT_WITH 1366
#define SCREEN_DEFAULT_HIGH 768

#define UP_BORDER_HIGH 50

#define RIGHT 0
#define LEFT 1
#define UP 2

class Color{
public:
    int r,g,b;
    Color(int r,int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }

};


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

   
    this->font = TTF_OpenFont("resources/Amiko-Bold.ttf", 15);
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
    Uint32 colorkey = SDL_MapRGBA(screen->format, 0, 255, 0,0.5);
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


//////////////////////////////////////////////////////////////////////
SDL_Window *second = SDL_CreateWindow("My Game Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          640, 480, 0);






    Graphic_Designer graphic_designer(1);

 



    SDL_Event event;

    bool running=true;
    while(running ){

         SDL_PollEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                cout << "se apreto x -> fin" << endl;
                return false;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    return false;
            }
        }

        Color player_color(255,0,0);

        graphic_designer.show_circle_life(screen, 0,500,500,player_color);

        //actualiza el dibujo de la superficie en la pantalla
        SDL_Flip(screen);
    }

    return 0;
}
