/*
 * Colour.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "Colour.h"

Colour::Colour(int r,int g, int b){
	this->r = r;
	this->g = g;
	this->b = b;
}

Colour Colour::create(Colour_name colour_name){
        switch(colour_name){
            case(White):{
                Colour white(255,255,255);
                return white;
            }
            case(Orange):{
                Colour orange(244, 95, 66);
                return orange;
            }
            case(Yellow):{
                Colour yellow(229, 220, 57);
                return yellow;
            }
            case(Green):{
                Colour green(57, 229, 114);
                return green;
            }
            case Blue:{
                Colour blue(83, 144, 237);
                return blue;
            }
            case Purple:{
                Colour purple(166, 109, 219);
                return purple;
            }
            case Pink:{
                Colour pink(239, 115, 198);
                return pink;
            }
            case Red:{
                Colour red(237, 78, 102);
                return red;
            }
            case Black:
                Colour black(0,0,0);
                return black;
        }
        Colour black(0,0,0);
        return black;

    }




Colour::~Colour() {
}

