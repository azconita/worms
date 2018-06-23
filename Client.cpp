#include "Client.h"



float Client::get_pixels(float meter_position){
    return  PIXEL_CONSTANT*meter_position;
}

Client::Client(char * host_name, char * port, std::string &stage, bool fullscreen)://
    socket(host_name, port),
    actions_queue(QUEUE_SIZE),
    stages_queue(QUEUE_SIZE),
    fullscreen(fullscreen){
    this->socket.connect_to_server();
    printf("connected, send: %s\n", stage.c_str());
    this->socket.send_dto(stage);
}


StageDTO Client::get_stage(){
    return this->stages_queue.pop(); 
}


void Client::run(){

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Error("No se pudo iniciar SDL: ",SDL_GetError());
    }

    atexit(SDL_Quit);
    int screen_width = SCREEN_DEFAULT_WIDTH;
    int screen_height = SCREEN_DEFAULT_HIGH;

    if(this->fullscreen){
        const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
        screen_width = info->current_w;
        screen_height = info->current_h;
    }

    oLog() << "Se crea la ventana de juego, height: " << screen_height << ", with: " << screen_width;


    if(SDL_VideoModeOK(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE) == 0) {
       throw Error("Modo video no compatible: ",SDL_GetError());
    }

    this->screen = SDL_SetVideoMode(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE);
    if(this->screen == NULL) {
       throw Error("No se pudo establecer el modo de video: ",SDL_GetError());
    }


    StageReceiver stage_receiver(this->socket,this->stages_queue);
    stage_receiver.start();

    // Set the title bar
    SDL_WM_SetCaption(TITLE, TITLE);

    StageDTO s = get_stage();
    this->id = s.player_id;


    printf("[Client] 1\n");
    GraphicDesigner graphic_designer(screen, screen_height,screen_width, s);
    printf("[Client] 2\n");
    std::map<int,WormAnimation>::iterator turn_worm_iter = graphic_designer.get_turn_worm(s.worm_turn);

    SDL_Event event;
    EventController event_controller(this->actions_queue,event, screen_height, screen_width, graphic_designer, this->id);
    

    Actioner actioner(this->socket,this->actions_queue);
    actioner.start();


    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;

    bool finish = false;
    bool running=true;
    while(running ){

        running = event_controller.continue_running(turn_worm_iter->second);
        //actualiza el dibujo de la superficie en la pantalla
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();

        //update
        if(finish == false){
            s = get_stage();
            turn_worm_iter = graphic_designer.get_turn_worm(s.worm_turn);
        } 


        if((t1 -t0) > 17) {


            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();
        
            //FIX ME
            if(s.winner != -1){
                finish = true;
                    if(s.winner == this->id){
                            printf("GANE\n");
                            SDL_Flip(screen);
                                graphic_designer.won();
                            
                    }else{
                            printf("PERDIII\n");
                            SDL_Flip(screen);
                            graphic_designer.lost();
                    }
            } else {
                  //borro todo lo que estaba
                //toda la pantalla en negro
                SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
                //dibujo las vigas y el agua
                graphic_designer.show_elements(s,screen);

            }

        }
    }
}
       



Client::~Client(){
    SDL_FreeSurface(this->screen);
    this->socket.shut();
}
