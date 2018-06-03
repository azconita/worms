/*
 * WormAnimationController.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef WORMANIMATIONCONTROLLER_H_
#define WORMANIMATIONCONTROLLER_H_
#include "Animation.h"

#define GRADES_PER_STEP 5.8064516129 //180/31


enum State{
   Still,
   Walk,
   Fall,
   Jump_state,
   Worm_missile,
   Worm_banana,
   Worm_bat,
   Worm_green_granade,
   Worm_red_granade,
   Worm_holy_granade,
   Worm_teletrans,
   Worm_air_attack,
   Worm_dynamite
};
//////////////////////////////////////////////////////


static  std::map<Weapon_Name, State> weapons_states {
    {W_Air_Attack, Worm_air_attack},
    {W_Bazooka,Worm_missile},
    {Dynamite,Worm_dynamite},
    {Mortar,Worm_missile},
    {Green_Granade,Worm_green_granade},
    {Holy_Granade,Worm_holy_granade},
    {Red_Granade,Worm_red_granade},
    {Teletrans,Worm_teletrans},
    {Banana,Worm_banana},
    {Baseboll_Bat,Worm_bat}
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



class WormAnimationController {
	int x, y;
	Direction direction;
	State state;
	float degrees;
	int weapon_power;
	int timer;
	std::map<int,Animation> animations;
public:
	WormAnimationController(int initial_x, int initial_y, Direction initial_dir);
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
	void show(SDL_Surface * screen);
	virtual ~WormAnimationController();
};

#endif /* WORMANIMATIONCONTROLLER_H_ */
