/*
 * Colour.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef COLOUR_H_
#define COLOUR_H_
#include "Logger.h"

extern  logger oLog;

enum Colour_name{
    White,
    Orange,
    Green,
    Purple,
    Pink,
    Yellow,
    Red,
    Blue,
    Black
};


class Colour {
public:
	int r,g,b;
	Colour(int r,int g, int b);
	static Colour create(Colour_name color_name);
	virtual ~Colour();
};

#endif /* COLOUR_H_ */
