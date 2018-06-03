#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "WaterAnimation.h"
#include "WormAnimation.h"
#include "GraphicDesigner.h"
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




////////////////////////////////////////////////////////////////////////////////////////////7/////////


class Event_Controller{
    GraphicDesigner graphic_designer;
    int screen_height;
    int screen_width;
    Stage & stage;
    SDL_Event &  event;
    bool wait_for_destination_clicl;
    bool wait_for_weapon_click;
ActionDTO action;

float meters_conversor(int pixel){
    return (pixel+0.0)/23.5;

}
void air_attack(WormAnimation& turn_worm){
    turn_worm.take_weapon(W_Air_Attack);
    this->wait_for_destination_clicl = true;
    this->action.type = Take_weapon;
    this->action.weapon = W_Air_Attack;
    this->stage.make_action(this->action);
}

void bazooka(WormAnimation& turn_worm){
    turn_worm.take_weapon(W_Bazooka);
    this->action.type = Take_weapon;
    this->action.weapon = W_Bazooka;
    this->stage.make_action(this->action);
}

void dynamite(WormAnimation& turn_worm){
    turn_worm.take_weapon(Dynamite);
    this->action.type = Take_weapon;
    this->action.weapon = Dynamite;
    this->stage.make_action(this->action);
}

void green_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Green_Granade);
    this->action.type = Take_weapon;
    this->action.weapon = Green_Granade;
    this->stage.make_action(this->action);
}

void holy_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Holy_Granade);
    this->action.type = Take_weapon;
    this->action.weapon = Holy_Granade;
    this->stage.make_action(this->action);
}

void mortar(WormAnimation& turn_worm){
    turn_worm.take_weapon(Mortar);
    this->action.type = Take_weapon;
    this->action.weapon = Mortar;
    this->stage.make_action(this->action);
}

void red_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Red_Granade);
    this->action.type = Take_weapon;
    this->action.weapon = Red_Granade;
    this->stage.make_action(this->action);
}

void teletrans(WormAnimation& turn_worm){
    turn_worm.take_weapon(Teletrans);
    this->action.type = Take_weapon;
    this->action.weapon = Teletrans;
    this->stage.make_action(this->action);
}

void banana(WormAnimation& turn_worm){
    turn_worm.take_weapon(Banana);
    this->action.type = Take_weapon;
    this->action.weapon = Banana;
    this->stage.make_action(this->action);
}

void baseboll_bat(WormAnimation& turn_worm){
    turn_worm.take_weapon(Baseboll_Bat);
    this->action.type = Take_weapon;
    this->action.weapon = Baseboll_Bat;
    this->stage.make_action(this->action);
}

void shot(WormAnimation& turn_worm,int x, int y){
    if(turn_worm.has_weapon()){
        printf("%f %f\n", meters_conversor(x),meters_conversor(y));
        this->action.type = Shot_weapon;
        this->action.x = meters_conversor(x);
        this->action.y = meters_conversor(y);
        this->action.weapon_degrees = turn_worm.get_degrees();
        this->action.direction = turn_worm.get_direction();
        this->action.power = turn_worm.get_weapon_power();
        this->action.time_to_explode = turn_worm.get_timer();
        this->wait_for_destination_clicl = true;
        this->stage.make_action(this->action);
    }
    else{
        cout << "no tiene arma para disparar" << endl;
    }
}

void click(WormAnimation& turn_worm){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(turn_worm.has_weapon_to_click()){
        shot(turn_worm,x,y);
    }

    else if(this->wait_for_weapon_click){
        if(graphic_designer.is_inside_weapon_menu(x,y)){
            Weapon_Name  weapon = graphic_designer.choose_weapon(x,y);
            turn_worm.take_weapon(weapon);
            this->action.type = Take_weapon;
            this->action.weapon = weapon;
            this->stage.make_action(this->action);
        }
    }
    this->wait_for_weapon_click = false;

}

void up(WormAnimation& turn_worm){
    if(turn_worm.has_point_weapon()){
        float degrees = turn_worm.point_up_weapon();
        printf("%f\n",degrees );
    }else{
        turn_worm.change_state(Jump_state);
        this->action.type = Make_move;
        this->action.move = Jump;
        this->action.direction = turn_worm.get_direction();
        this->stage.make_action(this->action);
    }

}

void down(WormAnimation& turn_worm){
    if(turn_worm.has_point_weapon()){
        float degrees = turn_worm.point_down_weapon();
        printf("%f\n",degrees );
    }

}

void right(WormAnimation& turn_worm){
    turn_worm.change_direction(Right);
    this->action.type = Make_move;
    this->action.move = Walk_right;
    this->stage.make_action(this->action);

}

void left(WormAnimation& turn_worm){
    turn_worm.change_direction(Left);
    this->action.type = Make_move;
    this->action.move = Walk_left;
    this->stage.make_action(this->action);
}


void space(WormAnimation& turn_worm){
    int power = turn_worm.get_weapon_power();
    if(power >0 && power < 100){
        turn_worm.add_power();
    }
}




void mouse_motion(){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x > screen_width - 5 && y < 5){
        cout << "Se quiere elegir un arma:" << endl;
        printf("%i %i\n",x,y);
        this->wait_for_weapon_click = true;
    }
}

void weapon_shot(WormAnimation& turn_worm){
    shot(turn_worm,turn_worm.get_x(),turn_worm.get_y());

}






public:
Event_Controller(SDL_Event & event, Stage & stage, int screen_height, int screen_width, GraphicDesigner & graphic_designer):
        event(event),
        stage(stage),
        graphic_designer(graphic_designer){
            this->screen_height = screen_height;
            this-> screen_width = screen_width;
            this->wait_for_destination_clicl = false;
            this->wait_for_weapon_click = false;
            this->action.worm_id = 0;

}



bool continue_running(WormAnimation& turn_worm){
    if(this->wait_for_weapon_click){
        this->graphic_designer.show_weapons_menu(100);
    }

    if(SDL_PollEvent(&this->event) != 1){
        return true; // no hay nuevos event
    }
    switch(event.type){
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONUP:
            click(turn_worm);
            break;
        case SDL_MOUSEMOTION:
            mouse_motion();
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    return false;
                case SDLK_w:
                    cout << "disparo" << endl;
                    weapon_shot(turn_worm);
                    break;
                case SDLK_LEFT:
                    left( turn_worm);
                    break;
                case SDLK_RIGHT:
                    right( turn_worm);
                    break;
                case SDLK_UP:
                    up( turn_worm);
                    break;
                case SDLK_DOWN:
                    down( turn_worm);
                    break;
                //-----------potencia..................
                case SDLK_SPACE:
                    space( turn_worm);
                    break;
                case SDLK_0:
                    turn_worm.set_timer(0);
                    break;
                case SDLK_1:
                    turn_worm.set_timer(1);
                    break;
                case SDLK_2:
                    turn_worm.set_timer(2);
                    break;
                case SDLK_3:
                    turn_worm.set_timer(3);
                    break;
                case SDLK_4:
                    turn_worm.set_timer(4);
                    break;
                case SDLK_5:
                    turn_worm.set_timer(5);
                    break;
                case SDLK_6:
                    turn_worm.set_timer(6);
                    break;
                case SDLK_7:
                    turn_worm.set_timer(7);
                    break;
                case SDLK_8:
                    turn_worm.set_timer(8);
                    break;
                case SDLK_9:
                    turn_worm.set_timer(9);
                    break;
                //-------------ARMAS--------------------
                case SDLK_a:
                air_attack( turn_worm);
                    break;
                case SDLK_b:
                    bazooka( turn_worm);
                    break;
                case SDLK_d:
                    dynamite( turn_worm);
                    break;
                case SDLK_g:
                    green_granade( turn_worm);
                    break;
                case SDLK_h:
                    holy_granade( turn_worm);
                    break;
                case SDLK_m:
                    mortar( turn_worm);
                    break;
                case SDLK_r:
                    red_granade( turn_worm);
                    break;
                case SDLK_t:
                    teletrans( turn_worm);
                    break;
                case SDLK_u:
                    banana( turn_worm);
                    break;
                case SDLK_v:
                    baseboll_bat( turn_worm);
                    break;
                }
                break;
    }
    return true;
    }

};








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
    Event_Controller event_controller(event, stage, screen_height, screen_width, graphic_designer);

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
