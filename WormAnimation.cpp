/*
 * WormAnimation.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "WormAnimation.h"

WormAnimation::WormAnimation(int initial_x, int initial_y, Direction initial_dir, int player, int wormid) : player_id(player), worm_id(wormid) {
    this->animations = AnimationFactory::get_wormsAnimations();
    printf("tamaño del vector de estados de gusano %lu\n", this->animations.size() );

    this->x = initial_x;
    this-> y = initial_y;
    this->state = Still;
    this->direction = initial_dir;
    for(std::map<State,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){
        animation_iter->second.set_current_direction(initial_dir);
    }
    this->weapon_power = 0;
    this->timer = 5;
    this->in_movement = false;
}

bool WormAnimation::has_weapon(){
    return (this->state != Walk && this->state != Still && this->state != Jump_state && this->state != Fall);
}

bool WormAnimation::has_weapon_to_click(){
    return (this->state == Worm_teleport || this->state == Worm_air_attack);
}

bool WormAnimation::has_point_weapon(){ //armas con las que no se puede apuntar
    return std::find(weapons_states_with_degrees.begin(), weapons_states_with_degrees.end(), this->state) //
    != weapons_states_with_degrees.end();
}

void WormAnimation::change_direction(Direction direction){
    for(std::map<State,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){
        animation_iter->second.set_current_direction(direction);
    }
    this->direction = direction;
}

void WormAnimation::change_state(State state){
    this->state = state; //sigue en la misma dire que antes
     for(std::map<State,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){
        animation_iter->second.first_step();
    }
    if( std::find(weapons_states_with_power.begin(), weapons_states_with_power.end(), this->state) //
    != weapons_states_with_power.end() && this->weapon_power == 0){
        this->weapon_power = 10;
    }else{
        this->weapon_power = 0;
    }
}

void WormAnimation::take_weapon(Weapon_Name weapon){
    std::map<Weapon_Name,State>::iterator weapon_state = weapons_states.find(weapon);
    if(weapon_state->second == this->state){
        return;
    }
    this->timer = 5;
    this->degrees = -90;
    change_state(weapon_state->second);
}

float WormAnimation::point_down_weapon(){
    std::map<State,Animation>::iterator animation_iter = animations.find(this->state);
    if(animation_iter->second.point_down()){
        this->degrees-=GRADES_PER_STEP; //31 fotos/180 grados

    }
    return this->degrees;

}

float WormAnimation::point_up_weapon(){
    std::map<State,Animation>::iterator animation_iter = animations.find(this->state);
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
    return this->direction;
}

void WormAnimation::define_movement(int position_x, int position_y){
     if(this->x != position_x || this->y != position_y){
        this->in_movement = true;
    }else{
        in_movement = false;
    }
}



void WormAnimation::move(int position_x, int position_y, State state, Direction direction){
    this->define_movement(position_x,position_y);

    if(this->direction != direction){
        this->change_direction(direction);
    }

    if(this->state != state){
        change_state(state);
    }

    this->x = position_x;
    this->y = position_y;
    std::map<State,Animation>::iterator animation_iter = animations.find(this->state);
    if(this->state == Jump_state || this->state == Jump_back_state ||this->state == Walk  || this->state == Fall//
     || this->state == Walk_down || this->state == Walk_up || this->state == Worm_disappear){
        while(!animation_iter->second.continue_internal_movement()){ 
            // primero termino el movimiento que tenia
        }
    }
}

bool WormAnimation::is_in_movement(){
    return this->in_movement;
}

int WormAnimation::get_x(){ 
    return this->x; 
}
int WormAnimation::get_y(){
    return this->y ;
}

int WormAnimation::get_player_id() {
  return this->player_id;
}

int WormAnimation::get_id() {
  return this->worm_id;
}

void WormAnimation::show(SDL_Surface * screen, SDL_Rect camera_position){
    std::map<State,Animation>::iterator animation_iter = animations.find(this->state);
    animation_iter->second.draw(screen, this->x -camera_position.x, this->y -camera_position.y - WORM_OFFSET);
}


WormAnimation::~WormAnimation() {
	// TODO Auto-generated destructor stub
}

