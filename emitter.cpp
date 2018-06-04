/*#include <iostream>
#include "yaml-cpp/yaml.h"

using std::cout;
using std::endl;




struct ElementDTO { //puede ser un gusano, un arma o una viga
  float pos_x;
  float pos_y; //vertice superior izquierdo en metros
  float h;
  float w;
  float angle;
  int life;
  int player_id;
  //Weapon_Name weapon;
  int weapon;
  int timer = 0;
  bool explosion;
};

YAML::Emitter& operator << (YAML::Emitter& out, const ElementDTO& e) {
	out << YAML::BeginMap;
	out << YAML::Key << "pos_x";
	out << YAML::Value << e.pos_x;
	out << YAML::Key <<  "pos_y";
	out << YAML::Value << e.pos_y;
	out << YAML::Key <<  "h";
	out << YAML::Value << e.h;
	out << YAML::Key <<  "w";
	out << YAML::Value << e.w;
	out << YAML::Key <<  "angle";
	out << YAML::Value << e.angle;
	out << YAML::Key <<  "life";
	out << YAML::Value << e.life;
	out << YAML::Key <<  "player_id";
	out << YAML::Value << e.player_id;
	out << YAML::Key <<  "weapon";
	out << YAML::Value << e.weapon;
	out << YAML::Key <<  "timer";
	out << YAML::Value << e.timer;
	out << YAML::Key <<  "explosion";
	out << YAML::Value << e.explosion;
	out << YAML::EndMap;
  return out;
}


struct StageDTO {
  int worm_turn;
  std::map<int,ElementDTO> worms;
  std::vector<ElementDTO> beams;
  std::vector<ElementDTO> weapons;
};

YAML::Emitter& operator << (YAML::Emitter& out, const StageDTO& s) {
	out << YAML::BeginMap;
	out << YAML::Key << "worm_turn";
	out << YAML::Value << s.worm_turn;
	out << YAML::Key << "beams";
	out << YAML::Flow << s.beams;
	out << YAML::Key << "weapons";
	out << YAML::Flow << s.weapons;
	out << YAML::Key << "worms";
	out << s.worms;
	out << YAML::EndMap;
  return out;
}


int main(){
	ElementDTO elem1;
	elem1.pos_x = 3;
	elem1.pos_y = 2;
	elem1.h = 10;
	elem1.w = 30;
	elem1.angle = 98.9;
	elem1.life = 100;
	elem1.player_id =0;
	elem1.weapon = 4;
	elem1.timer = 3;
	elem1.explosion = 0;

	ElementDTO elem2;
	elem2.pos_x = 3;
	elem2.pos_y = 2;
	elem2.h = 10;
	elem2.w = 30;
	elem2.angle = 98.9;
	elem2.life = 100;
	elem2.player_id =0;
	elem2.weapon = 4;
	elem2.timer = 3;
	elem2.explosion = 0;

	StageDTO s;
	s.worm_turn = 1;
	s.beams.push_back(elem1);
	s.beams.push_back(elem2);
	s.weapons.push_back(elem1);
	s.worms[1] = elem1;



	YAML::Emitter out;
  	out << s;
   cout << "Here's the output YAML:\n" << out.c_str() << endl;
   return 0;
	
}*/