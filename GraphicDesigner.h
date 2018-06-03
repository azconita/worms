/*
 * GraphicDesigner.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef GRAPHICDESIGNER_H_
#define GRAPHICDESIGNER_H_
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include "Colour.h"
#include "DTOs.h"
#include "WormAnimation.h"


static  std::vector<Colour_name> possible_colors {
    Orange,
    Green,
    Purple,
    Pink,
    Yellow
};


//armas con timer
static const std::vector<Weapon_Name> weapons_with_timer(
    {Holy_Granade,
    Dynamite,
    Baseboll_Bat,
    Red_Granade,
    Banana}
);



class GraphicDesigner {
	SDL_Surface * screen;
	int screen_height;
	int screen_width;
	TTF_Font *font;
	TTF_Font *time_font;
	SDL_Surface *power_bar;
	SDL_Surface * weapons_menu;
	std::map<int,WormAnimation> worms;
	std::map<Weapon_Name,Animation> weapons;

	float get_pixels(float meter_position);
	std::map<int,WormAnimation> create_worms(StageDTO s);
	std::map<Weapon_Name,Animation> create_weapons();
	bool is_timer_weapon(Weapon_Name weapon);



public:
	GraphicDesigner(SDL_Surface * screen, int screen_height, int screen_width,StageDTO initial_stage);
	void show_beams(StageDTO s, SDL_Surface *screen);
	void show_worms(StageDTO s, SDL_Surface *screen);
	void show_weapon( StageDTO s,SDL_Surface * screen);
	std::map<int,WormAnimation>::iterator get_turn_worm(int i);
	void show_life(int life, int x, int y, Colour color);
	void show_powerbar(int power);
	void show_weapons_menu(int size);
	bool is_inside_weapon_menu(int x, int y);
	Weapon_Name choose_weapon(int x, int y);
	void show_timer(int second);
	virtual ~GraphicDesigner();
};

#endif /* GRAPHICDESIGNER_H_ */
