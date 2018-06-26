/*
 * WormAnimationController.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef WORMANIMATION_H_
#define WORMANIMATION_H_
#include "AnimationFactory.h"
#include <algorithm>
#include <map>
#include "Logger.h"


#define GRADES_PER_STEP 5.8064516129 //180/31
#define WORM_OFFSET 10

extern  logger oLog;


//armas con mira
static const std::vector<State> weapons_states_with_degrees(
    {Worm_missile,
    Worm_green_granade,
    Worm_holy_granade,
    Worm_red_granade,
    Worm_banana,
    Worm_bat,
    Worm_missile_up,
    Worm_green_granade_up,
    Worm_holy_granade_up,
    Worm_red_granade_up,
    Worm_banana_up,
    Worm_bat_up,
    Worm_missile_down,
    Worm_green_granade_down,
    Worm_holy_granade_down,
    Worm_red_granade_down,
    Worm_banana_down,
    Worm_bat_down}
);

//armas con intensidad
static const std::vector<State> weapons_states_with_power(
    {Worm_missile,
    Worm_green_granade,
    Worm_holy_granade,
    Worm_red_granade,
    Worm_banana,
    Worm_missile_up,
    Worm_green_granade_up,
    Worm_holy_granade_up,
    Worm_red_granade_up,
    Worm_banana_up,
    Worm_missile_down,
    Worm_green_granade_down,
    Worm_holy_granade_down,
    Worm_red_granade_down,
    Worm_banana_down}

);



class WormAnimation {
    int player_id = -1;
    int worm_id = -1;
	int x, y;
	Direction direction;
	State state;
	float degrees;
	float weapon_power;
	int timer;
	bool in_movement;
	std::map<State,Animation> animations;
public:
	WormAnimation(int initial_x, int initial_y, Direction initial_dir, int player, int wormid);
	bool has_weapon();
	bool has_weapon_to_click();
	bool has_point_weapon();
	void change_direction(Direction direction);
	void change_state(State state);
	void take_weapon(Weapon_Name weapon);
	float point_down_weapon();
	float point_up_weapon();
	float get_degrees();
	bool add_power();
	float get_weapon_power();
	void set_timer(int timer);
	int get_timer();
	Direction get_direction();
	void move(int position_x, int position_y, State state, Direction direction);
	void define_movement(int position_x, int position_y);
	int get_x();
	int get_y();
	bool is_in_movement();
	void show(SDL_Surface * screen, SDL_Rect camera_position);
	int get_player_id();
	int get_id();
	virtual ~WormAnimation();
};

#endif /* WORMANIMATION_H_ */
