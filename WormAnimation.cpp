/*
 * WormAnimation.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "WormAnimation.h"

WormAnimation::WormAnimation(int initial_x, int initial_y, Direction initial_dir){

    Animation worm_walk = AnimationFactory::get_worm_walk();
    this->animations.insert(std::pair<int,Animation>(Still,worm_walk));
    this->animations.insert(std::pair<int,Animation>(Walk,worm_walk));

    Animation worm_fall = AnimationFactory::get_worm_fall();
    this->animations.insert(std::pair<int,Animation>(Fall,worm_fall));

    Animation worm_jump = AnimationFactory::get_worm_jump();
    this->animations.insert(std::pair<int,Animation>(Jump_state,worm_jump));

    Animation worm_missile = AnimationFactory::get_worm_missile();
    this->animations.insert(std::pair<int,Animation>(Worm_missile,worm_missile));

    Animation worm_banana = AnimationFactory::get_worm_banana();
    this->animations.insert(std::pair<int,Animation>(Worm_banana,worm_banana));

    Animation worm_bat = AnimationFactory::get_worm_bat();
    this->animations.insert(std::pair<int,Animation>(Worm_bat,worm_bat));

    Animation worm_green_granade = AnimationFactory::get_worm_green_granade();
    this->animations.insert(std::pair<int,Animation>(Worm_green_granade,worm_green_granade));

    Animation worm_red_granade = AnimationFactory::get_worm_red_granade();
    this->animations.insert(std::pair<int,Animation>(Worm_red_granade,worm_red_granade));

    Animation worm_holy_granade = AnimationFactory::get_worm_holy_granade();
    this->animations.insert(std::pair<int,Animation>(Worm_holy_granade,worm_holy_granade));

    Animation worm_teletrans = AnimationFactory::get_worm_teletrans();
    this->animations.insert(std::pair<int,Animation>(Worm_teletrans,worm_teletrans));

    Animation worm_air_attack = AnimationFactory::get_worm_air_attack();
    this->animations.insert(std::pair<int,Animation>(Worm_air_attack,worm_air_attack));

    Animation worm_dynamite = AnimationFactory::get_worm_dynamite();
    this->animations.insert(std::pair<int,Animation>(Worm_dynamite,worm_dynamite));

    this->x = initial_x;
    this-> y = initial_y;
    this->state = Still;
    this->direction = initial_dir;
    for(std::map<int,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){
        animation_iter->second.set_current_direction(initial_dir);
    }
    this->weapon_power = 0;
    this->timer = 5;
}

bool WormAnimation::has_weapon(){
    return (this->state != Walk && this->state != Still && this->state != Jump_state && this->state != Fall);
}

bool WormAnimation::has_weapon_to_click(){
    return (this->state == Worm_teletrans || this->state == Worm_air_attack);
}

bool WormAnimation::has_point_weapon(){ //armas con las que no se puede apuntar
    return std::find(weapons_states_with_degrees.begin(), weapons_states_with_degrees.end(), this->state) //
    != weapons_states_with_degrees.end();
}

void WormAnimation::change_direction(Direction direction){
    for(std::map<int,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){
        animation_iter->second.set_current_direction(direction);
    }
    this->direction = direction;
    if(this->state == Still){
        change_state(Walk);
    }

}

void WormAnimation::change_state(State state){
    this->state = state; //sigue en la misma dire que antes
}

void WormAnimation::take_weapon(Weapon_Name weapon){
    this->timer = 5;
    this->degrees = -90;
    std::map<Weapon_Name,State>::iterator weapon_state = weapons_states.find(weapon);
    change_state(weapon_state->second);
    if( std::find(weapons_states_with_power.begin(), weapons_states_with_power.end(), this->state) //
    != weapons_states_with_power.end() && this->weapon_power == 0){
        this->weapon_power = 10;
    }else{
        this->weapon_power = 0;
    }




}

float WormAnimation::point_down_weapon(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    if(animation_iter->second.point_down()){
        this->degrees-=GRADES_PER_STEP; //31 fotos/180 grados

    }
    return this->degrees;

}

float WormAnimation::point_up_weapon(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    if(animation_iter->second.point_up()){
            this->degrees+=GRADES_PER_STEP; //31 fotos/180 grados
    }
    return this->degrees;
}

float WormAnimation::get_degrees(){
    return this->degrees;
}

bool WormAnimation::add_power(){
    if(this->weapon_power < 100){
        this->weapon_power +=1;
        return true;
    }
    return false;
}
int WormAnimation::get_weapon_power(){
    return this->weapon_power;
}

void WormAnimation::set_timer(int timer){
    this->timer = timer;
}

int WormAnimation::get_timer(){
    return this->timer;
}



Direction WormAnimation::get_direction(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    return animation_iter->second.get_current_direction();
}


void WormAnimation::move(int position_x, int position_y){
    if(this->x = position_x && this->y == y && this->state == Fall){ // se cayo sobre una viga
        this->state = Still;
    }
    if(this->state != Jump_state && position_y > this->y){ //aumenta el y, se cae
        this->state = Fall;
    }
    this->x = position_x;
    this->y = position_y;
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);

    if(this->state == Jump_state || this->state ==Walk){
        if(!animation_iter->second.continue_internal_movement()){
            this->state = Still;
        }
    }

}

int WormAnimation::get_x(){
    return this->x;
}
int WormAnimation::get_y(){
    return this->y;
}

void WormAnimation::show(SDL_Surface * screen){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    animation_iter->second.draw(screen, this->x, this->y);
}


WormAnimation::~WormAnimation() {
	// TODO Auto-generated destructor stub
}

