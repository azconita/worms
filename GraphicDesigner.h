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
#define _USE_MATH_DEFINES
#include <cmath>
#include "Colour.h"
#include "Dtos.h"
#include "WormAnimation.h"
#include "ResourcesDefinitions.h" 
#include "Camera.h"
#include "WaterAnimation.h"
#define INTER_TURN -2

static  std::vector<Colour_name> possible_colors {
    Orange,
    Green,
    Purple,
    Pink,
    Yellow
};


//armas con timer
static const std::vector<Weapon_Name> weapons_with_timer(
    {Holy_Grenade,
    Dynamite,
    Green_Grenade,
    Red_Grenade,
    Banana}
);



class GraphicDesigner {
	SDL_Surface * screen;
	SDL_Surface * background;
	SDL_Surface * winner;
	SDL_Surface * looser;
	Camera * camera;
	int screen_height;
	int screen_width;
	int last_worm_turn;
	TTF_Font *font;
	TTF_Font *time_font;
	SDL_Surface *power_bar;
	SDL_Surface *power_back;
	SDL_Surface * weapons_menu;
	SDL_Surface * arrow;
	std::map<int,WormAnimation> worms;
	std::map<Weapon_Name,Animation> weapons;
	std::vector<Picture> little_beams;
    std::vector<Picture> big_beams;
    WaterAnimation water;


	float get_pixels(float meter_position);
	std::map<int,WormAnimation> create_worms(StageDTO s);
	bool is_timer_weapon(Weapon_Name weapon);
	Picture inclinate_beam(std::vector<Picture> beams, float degree);

	
	
	void show_beams(StageDTO s, SDL_Surface *screen,SDL_Rect camera_position);
	void show_worms(StageDTO s, SDL_Surface *screen,SDL_Rect camera_position);
	void show_weapon(StageDTO s,SDL_Surface *screen,SDL_Rect camera_position);
	void show_life(int life, int worm_x, int worm_y, Colour color);
	void show_arrow(int worm_x, int worm_y);
	void show_weapon_point_direction(int x, int y, float degrees, Direction dir);


public:
	GraphicDesigner(SDL_Surface * screen, int screen_height, int screen_width,StageDTO initial_stage);
	std::map<int,WormAnimation>::iterator get_turn_worm(int i);
	void scroll(int x, int y);
	SDL_Rect get_camera_position();
	void show_elements(StageDTO s, SDL_Surface *screen);
	void show_powerbar(int power);
	void show_weapons_menu(StageDTO& s);
	bool is_inside_weapon_menu(int x, int y);
	void resize(int h, int w);
	Weapon_Name choose_weapon(int x, int y);
	void show_timer(int second);
	void won();
 	void lost();
	virtual ~GraphicDesigner();
};

#endif /* GRAPHICDESIGNER_H_ */

