/*
 * Constants.h
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#include <string>

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace Constants {

  extern const float gravity;
  extern const float time_step;
  extern const int velocity_iterations;
  extern const int position_iterations;

  //worm constants
  extern const int worm_initial_life;
  extern const float worm_walk_velocity;
  extern const float worm_jump_velocity;
  extern const float worm_density;
  extern const float worm_size; //lado del cuadrado

  //beam constants
  extern const float beam_height;
  extern const float beam_width;
  //weapons constants
  extern const float weapon_size;
  extern const float weapon_density;
  //bazooka
  extern const float bazooka_damage;
  extern const float bazooka_radius;
  //dynamite
  extern const float dynamite_damage;
  extern const float dynamite_radius;
  //air attack
  extern const float airattack_damage;
  extern const float airattack_radius;
  //mortar
  extern const float mortar_damage;
  extern const float mortar_radius;
  //green grenade
  extern const float greengrenade_damage;
  extern const float greengrenade_radius;
  //holy grenade
  extern const float holygrenade_damage;
  extern const float holygrenade_radius;
  //red grenade
  extern const float redgrenade_damage;
  extern const float redgrenade_radius;
  //banana
  extern const float banana_damage;
  extern const float banana_radius;
  //baseball bat
  extern const float baseballbat_damage;
  extern const float baseballbat_radius;


} /* namespace Constants */

#endif /* CONSTANTS_H_ */
