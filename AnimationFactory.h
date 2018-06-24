/*
 * AnimationFactory.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef ANIMATIONFACTORY_H_
#define ANIMATIONFACTORY_H_
#include <vector>
#include "Animation.h"
#include "Dtos.h"
#include "ResourcesDefinitions.h"

class AnimationFactory {
	 AnimationFactory();
	static Animation get_worm_walk();
    static Animation get_worm_walk_up();
    static Animation get_worm_walk_down();
	static Animation get_worm_jump();
    static Animation get_worm_jump_up();
    static Animation get_worm_jump_down();
    static Animation get_worm_jump_back();
    static Animation get_worm_fall();
    static Animation get_worm_missile();
     static Animation get_worm_missile_up();
      static Animation get_worm_missile_down();
    static Animation get_worm_bat();
     static Animation get_worm_bat_up();
      static Animation get_worm_bat_down();
    static Animation get_worm_banana();
    static Animation get_worm_banana_up();
    static Animation get_worm_banana_down();
    static Animation get_worm_green_granade();
      static Animation get_worm_green_granade_up();
        static Animation get_worm_green_granade_down();
    static Animation get_worm_red_granade();
    static Animation get_worm_red_granade_up();
    static Animation get_worm_red_granade_down();

    static Animation get_worm_holy_granade();
        static Animation get_worm_holy_granade_up();
            static Animation get_worm_holy_granade_down();

    static Animation get_worm_teleport();
     static Animation get_worm_teleport_up();
      static Animation get_worm_teleport_down();

    static Animation get_worm_air_attack();
    static Animation get_worm_air_attack_up();
    static Animation get_worm_air_attack_down();

    static Animation get_worm_dynamite();
      static Animation get_worm_dynamite_up();
        static Animation get_worm_dynamite_down();
    //---------Armas ---------------
     static Animation get_bazooka();
    static Animation get_mortar();
    static Animation get_banana();
    static Animation get_green_granade();
    static Animation get_red_granade();
    static Animation get_holy_granade();
    static Animation get_air_attack();
    static Animation get_dynamite();
    static Animation get_explosion();
    static Animation get_big_explosion();
    static Animation get_fragment();
public:
    static std::vector<Picture> get_little_beams();
    static std::vector<Picture> get_big_beams();
    static std::map<Weapon_Name,Animation> get_weapons();
    static std::map<State,Animation> get_wormsAnimations();

	virtual ~AnimationFactory();

};

#endif /* ANIMATIONFACTORY_H_ */
