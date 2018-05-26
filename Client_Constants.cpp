#include "Constants.h"
#include <yaml-cpp/yaml.h>


namespace Resources_def {
  YAML::Node resources_definitions = YAML::LoadFile("resources_definitions.yaml");

//un gusano caminando 
extern const string worm_walking = resources_definitions["worm_walking"].as<string>();
extern const int worm_walking_r = resources_definitions["worm_walking_r"].as<int>();
extern const int worm_walking_g = resources_definitions["worm_walking_g"].as<int>();
extern const int worm_walking_b = resources_definitions["worm_walking_b"].as<int>();
extern const int worm_walking_columns = resources_definitions["worm_walking_columns"].as<int>();
extern const int worm_walking_rows = resources_definitions["worm_walking_rows"].as<int>();

//una viga larga
extern const string beam = resources_definitions["beam"].as<string>();
extern const int beam_r = resources_definitions["beam_r"].as<int>();
extern const int beam_g = resources_definitions["beam_g"].as<int>();
extern const int beam_b = resources_definitions["beam_b"].as<int>();
extern const int beam_columns = resources_definitions["beam_columns"].as<int>();
extern const int beam_rows = resources_definitions["beam_rows"].as<int>();



} 
