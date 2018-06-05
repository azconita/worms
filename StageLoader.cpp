
#include "StageLoader.h"

StageLoader::StageLoader(string file_name){
  this->file_name = file_name;
}

Stage_y StageLoader::load_stage(){
  YAML::Node initial_stage = YAML::LoadFile(this->file_name);
  Stage_y s = initial_stage["stage"].as<Stage_y>();
 
  return s;

}


	



