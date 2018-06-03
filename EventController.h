/*
 * EventController.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef EVENTCONTROLLER_H_
#define EVENTCONTROLLER_H_
#include <SDL/SDL.h>
#include "GraphicDesigner.h"
#include "stage.h"

class EventController {
	GraphicDesigner graphic_designer;
	int screen_height;
	int screen_width;
	Stage & stage;
	SDL_Event &  event;
	ActionDTO action;
	bool wait_for_destination_clicl;
	bool wait_for_weapon_click;
	float meters_conversor(int pixel);
	void air_attack(WormAnimation& turn_worm);
	void bazooka(WormAnimation& turn_worm);
	void dynamite(WormAnimation& turn_worm);
	void green_granade(WormAnimation& turn_worm);
	void holy_granade(WormAnimation& turn_worm);
	void mortar(WormAnimation& turn_worm);
	void red_granade(WormAnimation& turn_worm);
	void teletrans(WormAnimation& turn_worm);
	void banana(WormAnimation& turn_worm);
	void baseboll_bat(WormAnimation& turn_worm);

	void shot(WormAnimation& turn_worm,int x, int y);

	void click(WormAnimation& turn_worm);

	void up(WormAnimation& turn_worm);
	void down(WormAnimation& turn_worm);
	void right(WormAnimation& turn_worm);
	void left(WormAnimation& turn_worm);


	void space(WormAnimation& turn_worm);
	void mouse_motion();
	void weapon_shot(WormAnimation& turn_worm);

public:
	EventController(SDL_Event & event, Stage & stage, int screen_height, int screen_width, GraphicDesigner & graphic_designer);
	bool continue_running(WormAnimation& turn_worm);

	virtual ~EventController();

};

#endif /* EVENTCONTROLLER_H_ */
