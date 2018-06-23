/*
 * EventController.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "EventController.h"



EventController::EventController(BlockingQueue<ActionDTO> & actions_queue, SDL_Event & event, 
    int screen_height, int screen_width, GraphicDesigner & graphic_designer, int id) :
        actions_queue(actions_queue),
        event(event),
        graphic_designer(graphic_designer),
        id(id) {
            this->screen_height = screen_height;
            this-> screen_width = screen_width;
            this->wait_for_destination_clicl = false;
            this->action.worm_id = 0;
            this->i = 0; //para borrar
}





bool EventController::continue_running(WormAnimation& turn_worm){
    printf("event controller %i\n", this->i);
    this->i++;
    if(SDL_PollEvent(&this->event) != 1){
           printf("no hay nuevos eventos\n");
        return keep_clicking(turn_worm); // no hay nuevos eventos
     
    }
    switch(event.type){
        case SDL_QUIT:
            quit();
            return false;
        case SDL_VIDEORESIZE:
            resize(event.resize);
            break;   
        case SDL_MOUSEBUTTONUP:
            click(turn_worm);
            break;
        case SDL_MOUSEMOTION:
            mouse_motion();
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_ESCAPE ){
                quit();
                return false;
            }
            if(this->id == turn_worm.get_player_id()) {
              movement(event, turn_worm);
              weapon_shortcuts(event, turn_worm);
              weapon_action(event, turn_worm);
            }
            //break;
    }
    return true;
}

bool EventController::keep_clicking(WormAnimation& turn_worm){
    switch(this->event.type){
        case SDL_KEYDOWN:{
            switch(this->event.key.keysym.sym){
                case SDLK_SPACE:{
                    space(turn_worm);
                    break;
                }      
            }
        }
    }
    return true; 

}


void EventController::movement(SDL_Event & event, WormAnimation& turn_worm){
    switch(event.key.keysym.sym){
        case SDLK_w:
            weapon_shot(turn_worm);
            break;
        case SDLK_LEFT:
            left( turn_worm);
            break;
        case SDLK_RIGHT:
            right( turn_worm);
            break;
        case SDLK_UP:
            up(turn_worm);
            break;
        case SDLK_DOWN:
            down(turn_worm);
            break;
        case SDLK_BACKSPACE:
            up_back(turn_worm);
            break;
    }
}


void EventController::weapon_action(SDL_Event & event, WormAnimation& turn_worm){
    switch(event.key.keysym.sym){
        case SDLK_SPACE:
             weapon_shot(turn_worm);
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
    }

}

void EventController::click(WormAnimation& turn_worm){
    int x, y;
    SDL_GetMouseState(&x, &y);

    if(graphic_designer.is_inside_weapon_menu(x,y)){
        printf("esta dentro del menu\n");
        Weapon_Name  weapon = graphic_designer.choose_weapon(x,y);
        turn_worm.take_weapon(weapon);
        this->action.type = Take_weapon;
        this->action.weapon = weapon;
        send_action(this->action);
    } else if(turn_worm.has_weapon_to_click()){
        SDL_Rect camera_pos = this->graphic_designer.get_camera_position();
        x = meters_conversor(x + camera_pos.x);
        y = meters_conversor(y + camera_pos.y);
        shot(turn_worm,x,y);
    }

}


void EventController::mouse_motion(){
    int x, y;
    SDL_GetMouseState(&x, &y);
   this->graphic_designer.scroll(x,y);
}



void EventController::shot(WormAnimation& turn_worm,int x, int y){
    if(turn_worm.has_weapon()){
        this->action.type = Shot_weapon;
        this->action.pos_x = x;
        this->action.pos_y = y;
        this->action.weapon_degrees = turn_worm.get_degrees();
        this->action.direction = turn_worm.get_direction();
        this->action.power = turn_worm.get_weapon_power();
        this->action.time_to_explode = turn_worm.get_timer();
        this->wait_for_destination_clicl = true;
        send_action(this->action);
    }
    else{
        cout << "no tiene arma para disparar" << endl;
    }
}



void EventController::up(WormAnimation& turn_worm){
    if(turn_worm.has_point_weapon()){
        float degrees = turn_worm.point_up_weapon();
        printf("%f\n",degrees );
    }else{
        turn_worm.change_state(Jump_state);
        this->action.type = Make_move;
        this->action.move = Jump;
        this->action.direction = turn_worm.get_direction();
        send_action(this->action);
    }

}

void EventController::up_back(WormAnimation& turn_worm){
    turn_worm.change_state(Jump_back_state);
    this->action.type = Make_move;
    this->action.move = Jump_back;
    this->action.direction = turn_worm.get_direction();
    send_action(this->action);


}

void EventController::down(WormAnimation& turn_worm){
    if(turn_worm.has_point_weapon()){
        float degrees = turn_worm.point_down_weapon();
        printf("%f\n",degrees );
    }   

}

void EventController::right(WormAnimation& turn_worm){
    this->action.type = Make_move;
    this->action.move = Walk_right;
    send_action(this->action);

}

void EventController::left(WormAnimation& turn_worm){
    this->action.type = Make_move;
    this->action.move = Walk_left;
    this->action.worm_id = turn_worm.get_id();
    send_action(this->action);
}


void EventController::space(WormAnimation& turn_worm){
    int power = turn_worm.get_weapon_power();
    if(power >0 && power < 100){
        turn_worm.add_power();
    }
}




void EventController::weapon_shot(WormAnimation& turn_worm){
    float x = meters_conversor(turn_worm.get_x());
    float y = meters_conversor(turn_worm.get_y());
    shot(turn_worm,x,y);

}



float EventController::meters_conversor(int pixel){
    return (pixel+0.0)/23.5;    
}

void EventController::send_action(ActionDTO action){
    (this->actions_queue).push(action);
}

void EventController::resize(SDL_ResizeEvent resize){
    this->graphic_designer.resize(resize.h, resize.w);
}


void EventController::quit(){
    this->action.type = Quit;
    send_action(this->action);
}




///shortcuts--------------------------------------------------------------------------------


void EventController::weapon_shortcuts(SDL_Event & event, WormAnimation& turn_worm){
    switch(event.key.keysym.sym){
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
}
void EventController::air_attack(WormAnimation& turn_worm){
    printf("[EventController]    click\n");
    turn_worm.take_weapon(W_Air_Attack);
    this->wait_for_destination_clicl = true;
    this->action.type = Take_weapon;
    this->action.weapon = W_Air_Attack;
    send_action(this->action);
}

void EventController::bazooka(WormAnimation& turn_worm){
    turn_worm.take_weapon(W_Bazooka);
    this->action.type = Take_weapon;
    this->action.weapon = W_Bazooka;
    send_action(this->action);
}

void EventController::dynamite(WormAnimation& turn_worm){
    turn_worm.take_weapon(Dynamite);
    this->action.type = Take_weapon;
    this->action.weapon = Dynamite;
    send_action(this->action);
}

void EventController::green_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Green_Grenade);
    this->action.type = Take_weapon;
    this->action.weapon = Green_Grenade;
    send_action(this->action);
}

void EventController::holy_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Holy_Grenade);
    this->action.type = Take_weapon;
    this->action.weapon = Holy_Grenade;
    send_action(this->action);
}

void EventController::mortar(WormAnimation& turn_worm){
    turn_worm.take_weapon(Mortar);
    this->action.type = Take_weapon;
    this->action.weapon = Mortar;
    send_action(this->action);
}

void EventController::red_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Red_Grenade);
    this->action.type = Take_weapon;
    this->action.weapon = Red_Grenade;
    send_action(this->action);
}

void EventController::teletrans(WormAnimation& turn_worm){
    turn_worm.take_weapon(Teleport);
    this->action.type = Take_weapon;
    this->action.weapon = Teleport;
    send_action(this->action);
}

void EventController::banana(WormAnimation& turn_worm){
    turn_worm.take_weapon(Banana);
    this->action.type = Take_weapon;
    this->action.weapon = Banana;
    send_action(this->action);
}

void EventController::baseboll_bat(WormAnimation& turn_worm){
    turn_worm.take_weapon(Baseball_Bat);
    this->action.type = Take_weapon;
    this->action.weapon = Baseball_Bat;
    send_action(this->action);
}

EventController::~EventController(){}


