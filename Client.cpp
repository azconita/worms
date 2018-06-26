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
  this->socket.send_dto(stage);
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

  // Set the title bar
  SDL_WM_SetCaption(TITLE, TITLE);

  StageReceiver stage_receiver(this->socket,this->stages_queue);
  stage_receiver.start();

  StageDTO s = this->stages_queue.pop();
  this->id = s.player_id;

  GraphicDesigner graphic_designer(screen, screen_height,screen_width, s, this->id);

  Renderer renderer(screen, this->stages_queue,graphic_designer, id);
  renderer.start();

  SDL_Event event;
  EventController event_controller(this->actions_queue,event, screen_height, screen_width, graphic_designer, id);

  Actioner actioner(this->socket,this->actions_queue);
  actioner.start();

  bool running=true;
  while(running ){
    std::map<int, WormAnimation>::iterator turn_worm_iter = graphic_designer.get_turn_worm(renderer.get_worm_turn());
    running = event_controller.continue_running(turn_worm_iter->second);

  }
}

Client::~Client(){
  SDL_FreeSurface(this->screen);
  this->socket.shut();
}
