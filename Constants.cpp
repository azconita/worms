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

  extern const int worm_initial_life = config["worm_initial_life"].as<int>();
  extern const float worm_walk_velocity = config["worm_walk_velocity"].as<float>();
  extern const float worm_density = config["worm_density"].as<float>();
  extern const float worm_size = config["worm_size"].as<float>();

} /* namespace Constants */
