/*
 * WaterAnimation.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "WaterAnimation.h"

WaterAnimation::WaterAnimation(int speed):
        colorkey(WATER_R,WATER_G,WATER_B),
        water_left(WATER, this->colorkey,WATER_COLUMNS,WATER_ROWS),
        water_right(WATER, this->colorkey,WATER_COLUMNS,WATER_ROWS)
    {
        this->speed = speed;
        this->show_counter = 0;
        this->water_right = water_right;
        this->water_left = water_left;

    }

void WaterAnimation::show(SDL_Surface * screen, int height){
        if(this->show_counter == this->speed){
            this->show_counter = 0;
            water_left.continue_internal_movement();
            water_right.continue_internal_movement();

        }else{
            this->show_counter +=1;
        }
        water_left.draw(screen,0,height-water_left.get_height());
        printf("h: %i\n agua:%i \n",height, this->water_left.get_height() );
        water_right.draw(screen,this->water_left.get_width()-2,height - this->water_right.get_height());

    }

WaterAnimation::~WaterAnimation() {
	// TODO Auto-generated destructor stub
}

