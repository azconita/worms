//g++ -std=c++11 prueba.cpp -lyaml-cpp -g

#include <iostream>
#include "yaml-cpp/yaml.h"

using std::cout;
using std::endl;

/*int main(){

	ElementDTO explosion;
	explosion.pos_x = 3;
	explosion.pos_y = 2;
	explosion.h = 10;
	explosion.w = 30;
	explosion.angle = 98.9;
	explosion.life = 100;
	explosion.player_id =0;
	explosion.weapon = Banana;
	explosion.timer = 3;
	explosion.explosion = 0;


	ElementDTO weapon1;
	weapon1.pos_x = 3;
	weapon1.pos_y = 2;
	weapon1.h = 10;
	weapon1.w = 30;
	weapon1.angle = 98.9;
	weapon1.life = 100;
	weapon1.player_id =0;
	weapon1.weapon =Banana;
	weapon1.timer = 3;
	weapon1.explosion = 0;
	
	StageDTO stage_sended;
	stage_sended.worm_turn = 1;
	

	//stage_sended.beams.push_back(explosion);
	//stage_sended.beams.push_back(weapon1);
	
	stage_sended.weapons.push_back(1);
	
	stage_sended.worms[1] = explosion;
	stage_sended.worms[2] = weapon1;

	printf("en el vector hay %lu\n", stage_sended.weapons.size() );

	
	YAML::Emitter out;
	out << YAML::BeginMap;
  	out << YAML::Key << "stage";
  	out << YAML::Value << stage_sended;
  	out << YAML::EndMap;
    cout << "se envia:\n'" << out.c_str() <<"'" << endl;



   YAML::Node yaml_received = YAML::Load(out.c_str());
   StageDTO stage_received = yaml_received ["stage"].as<StageDTO>();
   cout << "se recibe:" << endl;
   //cout << "turno:" << stage_received.worm_turn << endl;
 /* for(auto b: stage_received.beams){
  	  cout << "se levanto: beam_y { pos x: " << b.pos_x << ", pos y: " << b.pos_y << ", h: " << b.h <<", w: " << b.w <<  ", angle:" << b.angle << "}" << endl;
  
  }/*
  for(auto we: stage_received.weapons){
  	  cout << "se levanto: beam_y { pos x: " << we.pos_x << ", pos y: " << we.pos_y << ", h: " << we.h <<", w: " << we.w <<  ", angle:" << we.angle << "}" << endl;
  
  }

  for(auto & pair: stage_received.worms){
    cout << "worm id  " << pair.first << endl;

    ElementDTO w = pair.second;
    
   	cout << ",  x: "<< w.pos_x << " , y: "<< w.pos_y << ", h: " 
      << w.h <<", w: " << w.w <<  ", angle: " << w.angle <<", player id: "<< w.player_id << ", life: " << w.life <<" }"<< endl;

  }	

   return 0;
	
}*/


enum Weapon_Name {
    W_Air_Attack, //a
    W_Bazooka, //b
    Dynamite, //d
    Mortar, //m
    W_Fragment,
    Green_Grenade,//g
    Holy_Grenade,//h
    Red_Grenade,//r
    Teleport,//t
    Banana,//u
    Baseball_Bat,//v
    Explosion,
    None,
    W_Timer
};

struct ElementDTO { //puede ser un gusano, un arma o una viga
  float pos_x;
  float pos_y; //vertice superior izquierdo en metros
  float h;
  float w;
  float angle;
  int life;
  int player_id;
  Weapon_Name weapon;
  int timer = 0;
  bool explosion;
};

struct StageDTO {
  int worm_turn;
  std::vector<ElementDTO> beams;
  std::map<int,ElementDTO> worms;
  std::vector<ElementDTO> weapons;
};

namespace YAML {

template<>
struct convert<StageDTO> {
  static Node encode(const StageDTO& s) {
    Node node;
    node["worm_turn"] = s.worm_turn;
    node["worms"] = s.worms;
    node["beams"] = s.beams;
    node["weapons"] = s.weapons;
 
  
    return node;
  }
  static bool decode(const Node& node, StageDTO& s) {
    s.worm_turn = node["worm_turn"].as<int>();
    s.beams = node["beams"].as<std::vector<ElementDTO>>();
    s.weapons = node["weapons"].as<std::vector<ElementDTO>>();
    s.worms = node["worms"].as<std::map<int, ElementDTO>>();
    return true;
  }
};


template<>
struct convert<ElementDTO> {
  static Node encode(const ElementDTO& elem) {
    Node node;
    node["pos_x"] = elem.pos_x;
    node["pos_y"] = elem.pos_y;
    node["h"] = elem.h;
    node["w"] = elem.w;
    node["life"] = elem.life;
    node["player_id"] = elem.player_id;
    node["weapon"] = (int)elem.weapon;
    node["timer"] = elem.timer;
    node["explosion"] = elem.explosion;
    node["angle"] = elem.angle;
    return node;
  }

  static bool decode(const Node& node, ElementDTO& elem) {
    elem.pos_x = node["pos_x"].as<float>();
    elem.pos_y = node["pos_y"].as<float>();
    elem.h = node["h"].as<float>();
    elem.w = node["w"].as<float>();
    elem.life = node["life"].as<int>();
    elem.player_id = node["player_id"].as<int>();
    elem.weapon = static_cast<Weapon_Name>(node["weapon"].as<int>());
    elem.timer = node["timer"].as<int>();
    elem.explosion = node["explosion"].as<bool>();
    elem.angle = node["angle"].as<float>();
    return true;
  }
};

}

YAML::Emitter& operator << (YAML::Emitter& out, const ElementDTO& e) {
  printf("serializando element dto\n");
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


YAML::Emitter& operator << (YAML::Emitter& out, const StageDTO& s) {
  out << YAML::BeginMap;
  out << YAML::Key << "worm_turn";
  out << YAML::Value << s.worm_turn;
   out << YAML::Key << "beams";
  out << YAML::Value << s.beams;
  out << YAML::Key << "weapons";
  out << YAML::Value << s.weapons;
  out << YAML::Key << "worms";
  out << YAML::Value << s.worms;
  out << YAML::EndMap;
  return out;
}

int main(){

	StageDTO stage_sended;
	stage_sended.worm_turn = 1;

	ElementDTO beam1;
	beam1.pos_x = 32;
	beam1.pos_y = 21;
	beam1.h = 10;
	beam1.w = 30;
	beam1.angle = 98.9;

	stage_sended.beams.push_back(beam1);

	

	ElementDTO weapon1;
	weapon1.pos_x = 3;
	weapon1.pos_y = 2;
	weapon1.h = 10;
	weapon1.w = 30;
	weapon1.weapon =Banana;
	weapon1.timer = 3;

	ElementDTO explosion;
	explosion.pos_x = 3;
	explosion.pos_y = 2;
	explosion.h = 10;
	explosion.w = 30;
	explosion.weapon = Explosion;
	explosion.explosion = 2;
	
	
	stage_sended.weapons.push_back(weapon1);
	stage_sended.weapons.push_back(explosion);


	ElementDTO worm1;
	worm1.pos_x = 3;
	worm1.pos_y = 2;
	worm1.h = 10;
	worm1.w = 30;
	worm1.angle = 98.9;
	worm1.life = 100;
	worm1.player_id =0;

	ElementDTO worm2;
	worm2.pos_x = 3;
	worm2.pos_y = 2;
	worm2.h = 10;
	worm2.w = 30;
	worm2.angle = 0;
	worm2.life = 90;
	worm2.player_id =1;

	
	stage_sended.worms[1] = worm1;	
	stage_sended.worms[2] = worm2;

	YAML::Emitter out;
	out << YAML::BeginMap;
  	out << YAML::Key << "stage";
  	out << YAML::Value << stage_sended;
  	out << YAML::EndMap;

    cout << "se envia:\n'" << out.c_str() <<"'" << endl;


    YAML::Node yaml_received = YAML::Load(out.c_str());
    StageDTO stage_received = yaml_received ["stage"].as<StageDTO>();
    cout << "se recibe:" << endl;
   	cout << "turno:" << stage_received.worm_turn << endl;
   	cout << "beams:\n";
   	for(auto b: stage_received.beams){
  	  cout <<" { pos x: " << b.pos_x << ", pos y: " << b.pos_y << ", h: "
  	   << b.h <<", w: " << b.w <<  ", angle:" << b.angle << "}" << endl;
  
  	}
  	cout << "weapons:\n";	
  	for(auto we: stage_received.weapons){
  		cout << " { pos x: " << we.pos_x << ", pos y: " << we.pos_y << ", h: " 
  		<< we.h <<", w: " << we.w << ", weapon name: " <<  we.weapon<< ", explosion:" << we.explosion << "}" << endl;
  	}

  	cout << "worms:\n";

  	for(auto & pair: stage_received.worms){
    	cout << " worm id:  " << pair.first << endl;

    	ElementDTO w = pair.second;
    
   		cout << "  { x: "<< w.pos_x << " , y: "<< w.pos_y << ", h: " 
      	<< w.h <<", w: " << w.w <<  ", angle: " << w.angle <<", player id: "<< w.player_id << ", life: " << w.life <<" }"<< endl;

  	}	


}