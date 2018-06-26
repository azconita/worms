/*
 * Animation.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "Animation.h"

Animation::Animation(const char * bmp_path, Colour color,int columns, int rows):
    picture(bmp_path, color, columns, rows ){
        this->figures_num = columns * rows;
        this->step = 0;
        this->direction = Left;
}

int Animation::get_height(){
  return this->picture.get_height();
}

int Animation::get_width(){
        return this->picture.get_width();
    }

void Animation::first_step(){
    while(!this->picture.is_in_first_figure(this->direction)){
        this->picture.previous_sprite_figure(this->direction);
    }
}

void Animation::set_current_direction(Direction direction){
    this->direction = direction;
    this->first_step();

}

Direction Animation::get_current_direction(){
    return this->direction;
}

void Animation::draw(SDL_Surface *screen, int x, int y){
	SDL_Rect position;
        position.x = x;
        position.y = y;
        this->picture.draw(screen, position, this->direction);
    }

bool Animation::continue_internal_movement(){
    this->picture.next_sprite_figure(this->direction);
    this->step += 1;
    if(this->step == this->figures_num){
        this->step = 0;
        return false;
    }
    return true;
}

bool Animation::point_up(){
    if(!this->picture.is_in_last_figure(this->direction)){
        this->picture.next_sprite_figure(this->direction);
        return true;
    }
    return false;

}
bool Animation::point_down(){
    if(!this->picture.is_in_first_figure(this->direction)){
        this->picture.previous_sprite_figure(this->direction);
        return true;
    }
    return false;
}

void Animation::add_degres(float degrees){
    this->first_step();
    int i = 0;
    while(degrees > 0){
        this->picture.next_sprite_figure(this->direction);
        degrees -= GRADES_PER_WEAPON_STEP;
        i++;
    }
}

Animation::~Animation() {
}

