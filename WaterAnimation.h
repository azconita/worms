/*
 * WaterAnimation.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef WATERANIMATION_H_
#define WATERANIMATION_H_
#include "Animation.h"

class WaterAnimation {
    Colour colorkey;
    Animation water_left;
    Animation water_right;
    int screen_height;
    int show_counter;
    int speed;

public:
    WaterAnimation(int screen_height,int speed);
    void show(SDL_Surface * screen);
	virtual ~WaterAnimation();
};

#endif /* WATERANIMATION_H_ */
