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


/*Constants::Constants(std::string configfile) {
  YAML::Node config = YAML::LoadFile(configfile);
  this->gravity = config["gravity"].as<float>();
  this->time_step = config["time_step"].as<float>();
}

Constants::~Constants() {
  // TODO Auto-generated destructor stub
}*/

} /* namespace Constants */
