/*
 * EventController.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "EventController.h"



EventController::EventController(Socket & socket, SDL_Event & event, int screen_height, int screen_width, GraphicDesigner & graphic_designer):
        socket(std::move(socket)),
        event(event),
        graphic_designer(graphic_designer){
            this->screen_height = screen_height;
            this-> screen_width = screen_width;
            this->wait_for_destination_clicl = false;
            this->wait_for_weapon_click = false;
            this->action.worm_id = 0;
}



bool EventController::continue_running(WormAnimation& turn_worm){
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
            if(event.key.keysym.sym == SDLK_ESCAPE ){
            	return false;
            }
            movement(event, turn_worm);
            weapon_shortcuts(event, turn_worm);
            weapon_action(event, turn_worm);
            break;
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
            up( turn_worm);
            break;
        case SDLK_DOWN:
            down( turn_worm);
            break;
    }
}


void EventController::weapon_action(SDL_Event & event, WormAnimation& turn_worm){
    switch(event.key.keysym.sym){
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
    }

}

void EventController::click(WormAnimation& turn_worm){
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
            (this->socket).send_dto(this->action);
        }
    }
    this->wait_for_weapon_click = false;

}


void EventController::mouse_motion(){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x > screen_width - 5 && y < screen_height/2){
        cout << "Se quiere elegir un arma:" << endl;
        printf("%i %i\n",x,y);
        this->wait_for_weapon_click = true;
    }
}



void EventController::shot(WormAnimation& turn_worm,int x, int y){
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
        (this->socket).send_dto(this->action);
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
        (this->socket).send_dto(this->action);
    }

}

void EventController::down(WormAnimation& turn_worm){
    if(turn_worm.has_point_weapon()){
        float degrees = turn_worm.point_down_weapon();
        printf("%f\n",degrees );
    }

}

void EventController::right(WormAnimation& turn_worm){
    turn_worm.change_direction(Right);
    this->action.type = Make_move;
    this->action.move = Walk_right;
    (this->socket).send_dto(this->action);

}

void EventController::left(WormAnimation& turn_worm){
    turn_worm.change_direction(Left);
    this->action.type = Make_move;
    this->action.move = Walk_left;
    (this->socket).send_dto(this->action);
}


void EventController::space(WormAnimation& turn_worm){
    int power = turn_worm.get_weapon_power();
    if(power >0 && power < 100){
        turn_worm.add_power();
    }
}




void EventController::weapon_shot(WormAnimation& turn_worm){
    shot(turn_worm,turn_worm.get_x(),turn_worm.get_y());

}



float EventController::meters_conversor(int pixel){
    return (pixel+0.0)/23.5;

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
    turn_worm.take_weapon(W_Air_Attack);
    this->wait_for_destination_clicl = true;
    this->action.type = Take_weapon;
    this->action.weapon = W_Air_Attack;
    (this->socket).send_dto(this->action);
}

void EventController::bazooka(WormAnimation& turn_worm){
    turn_worm.take_weapon(W_Bazooka);
    this->action.type = Take_weapon;
    this->action.weapon = W_Bazooka;
    (this->socket).send_dto(this->action);
}

void EventController::dynamite(WormAnimation& turn_worm){
    turn_worm.take_weapon(Dynamite);
    this->action.type = Take_weapon;
    this->action.weapon = Dynamite;
    (this->socket).send_dto(this->action);
}

void EventController::green_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Green_Grenade);
    this->action.type = Take_weapon;
    this->action.weapon = Green_Grenade;
    (this->socket).send_dto(this->action);
}

void EventController::holy_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Holy_Grenade);
    this->action.type = Take_weapon;
    this->action.weapon = Holy_Grenade;
    (this->socket).send_dto(this->action);
}

void EventController::mortar(WormAnimation& turn_worm){
    turn_worm.take_weapon(Mortar);
    this->action.type = Take_weapon;
    this->action.weapon = Mortar;
    (this->socket).send_dto(this->action);
}

void EventController::red_granade(WormAnimation& turn_worm){
    turn_worm.take_weapon(Red_Grenade);
    this->action.type = Take_weapon;
    this->action.weapon = Red_Grenade;
    (this->socket).send_dto(this->action);
}

void EventController::teletrans(WormAnimation& turn_worm){
    turn_worm.take_weapon(Teleport);
    this->action.type = Take_weapon;
    this->action.weapon = Teleport;
    (this->socket).send_dto(this->action);
}

void EventController::banana(WormAnimation& turn_worm){
    turn_worm.take_weapon(Banana);
    this->action.type = Take_weapon;
    this->action.weapon = Banana;
    (this->socket).send_dto(this->action);
}

void EventController::baseboll_bat(WormAnimation& turn_worm){
    turn_worm.take_weapon(Baseball_Bat);
    this->action.type = Take_weapon;
    this->action.weapon = Baseball_Bat;
    (this->socket).send_dto(this->action);
}

EventController::~EventController(){}


