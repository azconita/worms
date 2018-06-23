#ifndef WEAPONANIMATION_H_
#define WEAPONANIMATION_H_
#include "AnimationFactory.h"
#include <algorithm>
#include <map>


#define GRADES_PER_STEP 5.8064516129 //180/31




class WeaponAnimation {
	int x, y;
	float degrees;
	std::map<Weapon_name,Animation> animations;

public:
	WeaponAnimation(int initial_x, int initial_y, float initial_degrees);
	void show(SDL_Surface * screen, SDL_Rect camera_position, float degrees);
	virtual ~WeaponAnimation();
};