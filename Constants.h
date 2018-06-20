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
  extern const int total_players;

  extern const float gravity;
  extern const float time_step;
  extern const int velocity_iterations;
  extern const int position_iterations;
  extern const float wind;

  //worm constants
  extern const int worm_turn_time;
  extern const int worm_initial_life;
  extern const float worm_walk_velocity;
  extern const float worm_jump_velocity;
  extern const float worm_density;
  extern const float worm_friction; 
  extern const float worm_height; 
  extern const float worm_width;

  //beam constants
  extern const float beam_height;
  extern const float beam_width;
  extern const float beam_friction;
  //weapons constants
  extern const float explosion_power;
  extern const float weapon_size;
  extern const float weapon_density;
  //bazooka
  extern const float bazooka_damage;
  extern const float bazooka_radius;
  extern const float bazooka_ammo;
  //dynamite
  extern const float dynamite_damage;
  extern const float dynamite_radius;
  extern const float dynamite_ammo;
  //air attack
  extern const float airattack_damage;
  extern const float airattack_radius;
  extern const float airattack_ammo;
  //mortar
  extern const float mortar_damage;
  extern const float mortar_radius;
  extern const float mortar_ammo;
  //fragment
  extern const float fragment_damage;
  extern const float fragment_radius;
  //green grenade
  extern const float greengrenade_damage;
  extern const float greengrenade_radius;
  extern const float greengrenade_ammo;
  //holy grenade
  extern const float holygrenade_damage;
  extern const float holygrenade_radius;
  extern const float holygrenade_ammo;
  //red grenade
  extern const float redgrenade_damage;
  extern const float redgrenade_radius;
  extern const float redgrenade_ammo;
  //banana
  extern const float banana_damage;
  extern const float banana_radius;
  extern const float banana_ammo;
  //baseball bat
  extern const float baseballbat_damage;
  extern const float baseballbat_radius;
  extern const float baseballbat_ammo;
  //teleport
  extern const float teleport_ammo;

} /* namespace Constants */

#endif /* CONSTANTS_H_ */
