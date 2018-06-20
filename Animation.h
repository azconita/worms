/*
 * Animation.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Picture.h"

class Animation {
	 Picture picture;
	 SDL_Rect dimention;
	 int figures_num;
	 int step;
	 Direction direction;
public:
	Animation(const char * bmp_path, Colour color,int columns, int rows);
	int get_height();
	int get_width();
	virtual ~Animation();
	void set_current_direction(Direction direction);
	Direction get_current_direction();
	void draw(SDL_Surface *screen, int x, int y);
	bool continue_internal_movement();
	void first_step();
	bool point_up();
	bool point_down();
};

#endif /* ANIMATION_H_ */
