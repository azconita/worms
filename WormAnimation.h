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


#define GRADES_PER_STEP 5.8064516129 //180/31


static  std::map<Weapon_Name, State> weapons_states {
    {W_Air_Attack, Worm_air_attack},
    {W_Bazooka,Worm_missile},
    {Dynamite,Worm_dynamite},
    {Mortar,Worm_missile},
    {Green_Grenade,Worm_green_granade},
    {Holy_Grenade,Worm_holy_granade},
    {Red_Grenade,Worm_red_granade},
    {Teleport,Worm_teletrans},
    {Banana,Worm_banana},
    {Baseball_Bat,Worm_bat}
};


//armas con mira
static const std::vector<State> weapons_states_with_degrees(
    {Worm_missile,
    Worm_green_granade,
    Worm_holy_granade,
    Worm_red_granade,
    Worm_banana,
    Worm_bat}
);

//armas con intensidad
static const std::vector<State> weapons_states_with_power(
    {Worm_missile,
    Worm_green_granade,
    Worm_holy_granade,
    Worm_red_granade,
    Worm_banana}
);



class WormAnimation {
    int player_id = -1;
    int worm_id = -1;
	int x, y;
	Direction direction;
	State state;
	float degrees;
	int weapon_power;
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
	int get_weapon_power();
	void set_timer(int timer);
	int get_timer();
	Direction get_direction();
	void move(int position_x, int position_y);
	int get_x();
	int get_y();
	bool is_in_movement();
	void show(SDL_Surface * screen, SDL_Rect camera_position);
	int get_player_id();
	int get_id();
	virtual ~WormAnimation();
};

#endif /* WORMANIMATION_H_ */
