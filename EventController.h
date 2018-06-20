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
#include "BlockingQueue.h"
#include "Socket.h"
#include "Dtos.h"

class EventController {

	GraphicDesigner & graphic_designer;
	BlockingQueue<ActionDTO> & actions_queue;
	SDL_Event &  event;
	ActionDTO action;
	int screen_height;
	int screen_width;
	int id;
	int i;
	
	bool wait_for_destination_clicl;

public:
	EventController(BlockingQueue<ActionDTO> & actions_queue, SDL_Event & event, int screen_height, int screen_width, 
		GraphicDesigner & graphic_designer, int id);
	bool continue_running(WormAnimation& turn_worm);
	virtual ~EventController();


private:

	void send_action(ActionDTO action);

	void weapon_shortcuts(SDL_Event & event, WormAnimation& turn_worm);
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
	void take_weapon(WormAnimation& turn_worm, Weapon_Name weapon);

	void click(WormAnimation& turn_worm);
	bool keep_clicking(WormAnimation& turn_worm);
	void resize(SDL_ResizeEvent resize);

	void movement(SDL_Event & event, WormAnimation& turn_worm);
	void up(WormAnimation& turn_worm);
	void up_back(WormAnimation& turn_worm);
	void down(WormAnimation& turn_worm);
	void right(WormAnimation& turn_worm);
	void left(WormAnimation& turn_worm);

	void weapon_action(SDL_Event & event, WormAnimation& turn_worm);
	void space(WormAnimation& turn_worm);
	void mouse_motion();
	void weapon_shot(WormAnimation& turn_worm);
	void shot(WormAnimation& turn_worm,int x, int y);

	void quit();


};

#endif /* EVENTCONTROLLER_H_ */
