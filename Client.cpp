#include "Client.h"


float Client::get_pixels(float meter_position){
    return  PIXEL_CONSTANT*meter_position;
}

void Client::debug_box2d_figure(SDL_Surface *screen, ElementDTO element_info){
    //dibujo un rectangulo
    SDL_Rect rectangle;
    rectangle.x = get_pixels(element_info.pos_x);
    rectangle.y = get_pixels(element_info.pos_y);
    rectangle.h = get_pixels(element_info.h);
    rectangle.w = get_pixels(element_info.w);
    Uint32 colorkey = SDL_MapRGBA(screen->format, 0, 255, 0,0.5);
    SDL_FillRect(screen, &rectangle, colorkey);

}

Client::Client(char * host_name, char * port)://
    socket(host_name, port),
    actions_queue(1000){  
    this->socket.connect_to_server();
}


StageDTO Client::receive_stage(){
    string stage_str = (this->socket).receive_dto();
    //printf("%s\n", stage_str.c_str());
    YAML::Node yaml_received = YAML::Load(stage_str);
    StageDTO stage_received = yaml_received ["stage"].as<StageDTO>();
    printf("worm turn: %d\n", stage_received.worm_turn);
    return stage_received;
}


void Client::run(){  

    extern  logger oLog; 


    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Error("No se pudo iniciar SDL: ",SDL_GetError());
    }

    atexit(SDL_Quit);
    int screen_width = SCREEN_DEFAULT_WITH;
    int screen_height = SCREEN_DEFAULT_HIGH;

    const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
    screen_width = info->current_w;
    screen_height = info->current_h;

    oLog() << "Se crea la ventana de juego, height: " << screen_height << ", with: " << screen_width;


    if(SDL_VideoModeOK(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF) == 0) {
       throw Error("Modo video no compatible: ",SDL_GetError());
    }

    // Establecemos el modo de video
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(screen == NULL) {
       throw Error("No se pudo establecer el modo de video: ",SDL_GetError());
    }

    // Set the title bar
    SDL_WM_SetCaption(TITLE, TITLE);

    StageDTO s = receive_stage();

    WaterAnimation water(screen_height, 3);

    GraphicDesigner graphic_designer(screen, screen_height,screen_width, s);
    
    //turno harcodeado
    printf("worm turn: %d", s.worm_turn);
    std::map<int,WormAnimation>::iterator turn_worm_iter = graphic_designer.get_turn_worm(s.worm_turn);

    SDL_Event event;
    EventController event_controller(this->actions_queue,event, screen_height, screen_width, graphic_designer);
    Actioner actioner(this->socket,this->actions_queue);
    actioner.start();

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
        StageDTO s = receive_stage();
        turn_worm_iter = graphic_designer.get_turn_worm(s.worm_turn);

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
}

Client::~Client(){
    this->socket.shut();
}

