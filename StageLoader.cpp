
#include "StageLoader.h"

StageLoader::StageLoader(string file_name){
  this->file_name = file_name;
}

Stage_y StageLoader::load_stage(){
  YAML::Node initial_stage = YAML::LoadFile(this->file_name);
  Stage_y s = initial_stage["stage"].as<Stage_y>();
  for(auto b: s.beams){
    cout << "se levanto: beam_y { x: " << b.pos_x << ", y: " << b.pos_y << ", size: " << b.size << ", inclination:" << b.inclination << "}" << endl;
  }
  for(auto & pair: s.players){
    cout << "player  " << pair.first << endl;
    cout << "worms: ";
    for(auto w : pair.second){
      cout << "{ id: "<< w.id << ", x: "<< w.pos_x << " , y: "
      << w.pos_y << ", direction: "<< w.direction << ", inclination: "<<w.inclination << ", life: " << w.life <<" }"<< endl;
    }
  }
  return s;

}


	



