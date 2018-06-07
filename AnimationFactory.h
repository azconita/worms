/*
 * AnimationFactory.h
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#ifndef ANIMATIONFACTORY_H_
#define ANIMATIONFACTORY_H_
#include "Animation.h"
#include <vector>
#include "ResourcesDefinitions.h"

class AnimationFactory {
	 AnimationFactory();
public:

	static Animation get_worm_walk();
	static Animation get_worm_jump();
    static Animation get_worm_fall();
    static Animation get_worm_missile();
    static Animation get_worm_bat();
    static Animation get_worm_banana();
    static Animation get_worm_green_granade();
    static Animation get_worm_red_granade();
    static Animation get_worm_holy_granade();
    static Animation get_worm_teletrans();
    static Animation get_worm_air_attack();
    static Animation get_worm_dynamite();
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
    static std::vector<Picture> get_little_beams();
    static std::vector<Picture> get_big_beams();

	virtual ~AnimationFactory();

};

#endif /* ANIMATIONFACTORY_H_ */
