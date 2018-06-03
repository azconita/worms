#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "WaterAnimation.h"
#include "WormAnimation.h"
#include "EventController.h"
#include "stage.h"
#include "Beam.h"
#include "Worm.h"


using std::cout;
using std::endl;
using std::string;
using std::map;
using std::pair;




//////////////////////////////////////////////////////////////////////////////////////////

//para borrar
float get_pixels(float meter_position){
    return  23.5*meter_position;
}

void debug_box2d_figure(SDL_Surface *screen, ElementDTO element_info){

    //dibujo un rectangulo
    SDL_Rect rectangle;
    rectangle.x = get_pixels(element_info.x);
    rectangle.y = get_pixels(element_info.y);
    rectangle.h = get_pixels(element_info.h);
    rectangle.w = get_pixels(element_info.w);

    Uint32 colorkey = SDL_MapRGBA(screen->format, 0, 255, 0,0.5);
    SDL_FillRect(screen, &rectangle, colorkey);

}











//////////////////////////////////////////////////////////////////7


//////////////////////////////////////////////////////////////////////////////////////////



#define SCREEN_DEFAULT_WITH 1366
#define SCREEN_DEFAULT_HIGH 768

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

   
    //------------------------------------

    Stage stage("file.yaml");

    StageDTO s = stage.get_stageDTO();

    WaterAnimation water(screen_height, 3);


    //------------------------------------

    GraphicDesigner graphic_designer(screen, screen_height,screen_width, s);
    


    //turno harcodeado
    std::map<int,WormAnimation>::iterator turn_worm_iter = graphic_designer.get_turn_worm(0);



    SDL_Event event;
    EventController event_controller(event, stage, screen_height, screen_width, graphic_designer);

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
            water.show(screen);
            graphic_designer.show_beams(s, screen);

            //dibujo los gusanos
            graphic_designer.show_worms(s, screen);
            graphic_designer.show_weapon(s, screen);

        }

    }

    return 0;
}
