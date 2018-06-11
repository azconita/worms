/*
 * Constants.cpp
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#include "Constants.h"
#include <yaml-cpp/yaml.h>

namespace Constants {
  YAML::Node config = YAML::LoadFile("server_config.yaml");

  extern const float gravity = config["gravity"].as<float>();
  extern const float time_step = config["time_step"].as<float>();
  extern const int velocity_iterations = config["velocity_iterations"].as<int>();
  extern const int position_iterations = config["position_iterations"].as<int>();
  extern const float wind = config["wind"].as<float>();

  extern const int worm_initial_life = config["worm_initial_life"].as<int>();
  extern const int worm_turn_time = config["worm_turn_time"].as<int>();
  extern const float worm_walk_velocity = config["worm_walk_velocity"].as<float>();
  extern const float worm_jump_velocity = config["worm_jump_velocity"].as<float>();
  extern const float worm_density = config["worm_density"].as<float>();
  extern const float worm_size = config["worm_size"].as<float>();

  //beam constants
  extern const float beam_height = 0.8;
  extern const float beam_friction = config["beam_friction"].as<float>();
  extern const float beam_width = config["beam_width"].as<float>();
  //weapons constants
  extern const float weapon_size = config["weapon_size"].as<float>();
  extern const float weapon_density = config["weapon_density"].as<float>();
  //bazooka
  extern const float bazooka_damage = config["bazooka_damage"].as<float>();
  extern const float bazooka_radius = config["bazooka_radius"].as<float>();
  //dynamite
  extern const float dynamite_damage = 50;
  extern const float dynamite_radius = 4;
  extern const float dynamite_ammo = 5;
  //air attack
  extern const float airattack_damage = 40;
  extern const float airattack_radius = 2;
  extern const float airattack_ammo = 2;
  //mortar
  extern const float mortar_damage = 50;
  extern const float mortar_radius = 2;
  extern const float mortar_second_damage = 10;
  extern const float mortar_second_radius = 2;
  extern const float mortar_second_fragmets = 6;
  extern const float mortar_ammo = 10;
  //fragments
  extern const float fragment_damage = 10;
  extern const float fragment_radius = 2;
  //green grenade
  extern const float greengrenade_damage = 30;
  extern const float greengrenade_radius = 2;
  //holy grenade
  extern const float holygrenade_damage = 110;
  extern const float holygrenade_radius = 8;
  extern const float holygrenade_ammo = 2;
  //red grenade
  extern const float redgrenade_damage = 30;
  extern const float redgrenade_radius = 2;
  extern const float redgrenade_second_damage = 10;
  extern const float redgrenade_second_radius = 2;
  extern const float redgrenade_second_fragments = 6;
  extern const float redgrenade_ammo = 10;
  //banana
  extern const float banana_damage = 70;
  extern const float banana_radius = 4;
  extern const float banana_ammo = 5;
  //baseball bat
  extern const float baseballbat_damage = 10;
  extern const float baseballbat_radius = 1;


} /* namespace Constants */
