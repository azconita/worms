/*
 * Picture.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef PICTURE_H_
#define PICTURE_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include "Colour.h"
#include "Dtos.h"
#include "Error.h"
#include <cmath>
#include "Logger.h"

extern  logger oLog;

using std::cout;
using std::endl;

class Picture {
	int rows, columns;
	    int row_num, column_num;
	    int w, h;

	    Direction default_direction;
	    SDL_Surface *surface;
	    SDL_Surface *flipped;
	SDL_Surface * flip(SDL_Surface * original, Uint32 colorkey);
	SDL_Rect get_dimention();

public:
	Picture(const char * bmp_path, Colour color, int columns, int rows);
	int get_height();
	int get_width();
	void draw(SDL_Surface *screen, int x, int y);
	void draw(SDL_Surface *screen, std::vector<Vertex> v, Direction direction);
	void draw(SDL_Surface *screen, SDL_Rect position, Direction direction);
	void debug(SDL_Surface *screen, std::vector<Vertex> v);
	bool is_in_first_figure(Direction direction);
	bool is_in_last_figure(Direction direction);
	void next_sprite_figure(Direction direction);
	void previous_sprite_figure(Direction direction);
	virtual ~Picture();
};

#endif /* PICTURE_H_ */
