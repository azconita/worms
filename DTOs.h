/*
 * DTOs.h
 *
 *  Created on: May 27, 2018
 *      Author: gabi
 */

#ifndef DTOS_H_
#define DTOS_H_

#include "yaml-cpp/yaml.h"
#include <map>
#include <vector>
#include <string>




enum Direction{
    Right,
    Left
};

enum Entity_Type {
  E_Beam,
  E_Worm,
  E_Weapon,
  E_Explosion
};

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


enum Movement {
  Walk_right,
  Walk_left,
  Jump,
  Jump_back
};
enum Action_Type {
  Make_move,
  Take_weapon,
  Shot_weapon
};

struct ActionDTO {
  int worm_id;
  Action_Type type;
  Movement move;
  Weapon_Name weapon;
  float weapon_degrees;
  int power;
  int time_to_explode;
  Direction direction;
  int pos_x;
  int pos_y;
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


template<>
struct convert<ActionDTO> {
  static Node encode(const ActionDTO& action) {
    Node node;
    node["worm_id"] = action.worm_id;
    node["type"] = (int) action.type;
    node["move"] = (int) action.move;
    node["weapon"] = (int) action.weapon;
    node["weapon_degrees"] =  action.weapon_degrees;
    node["power"] =  action.power;
    node["time_to_explode"] =  action.time_to_explode;
    node["direction"] = (int) action.direction;
    node["pos_x"] = action.pos_x;
    node["pos_y"] = action.pos_y;
    return node;
  }

  static bool decode(const Node& node, ActionDTO& action) {

    action.worm_id = node["worm_id"].as<int>(); 
    action.type = static_cast<Action_Type>(node["type"].as<int>());
    action.move = static_cast<Movement>(node["move"].as<int>());
    action.weapon = static_cast<Weapon_Name>(node["weapon"].as<int>());
    action.weapon_degrees = node["weapon_degrees"].as<float>(); 
    action.power = node["power"].as<int>();
    action.time_to_explode= node["time_to_explode"].as<int>(); 
    action.direction = static_cast<Direction>(node["direction"].as<int>()); 
  
    action.pos_x = node["pos_x"].as<int>();
    action.pos_y = node["pos_y"].as<int>();
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


YAML::Emitter& operator << (YAML::Emitter& out, const ActionDTO& a) {
  out << YAML::BeginMap;
  out << YAML::Key << "worm_id";
  out << YAML::Value << a.worm_id;
  out << YAML::Key << "type";
  out << YAML::Value << a.type;
  out << YAML::Key << "move";
  out << YAML::Value << a.move;
  out << YAML::Key << "weapon";
  out << YAML::Value << a.weapon;
  out << YAML::Key << "weapon_degrees";
  out << YAML::Value << a.weapon_degrees;
  out << YAML::Key << "power";
  out << YAML::Value << a.power;
  out << YAML::Key << "time_to_explode";
  out << YAML::Value << a.time_to_explode;
  out << YAML::Key << "direction";
  out << YAML::Value << a.direction;
  out << YAML::Key << "pos_x";
  out << YAML::Value << a.pos_x;
  out << YAML::Key <<  "pos_y";
  out << YAML::Value << a.pos_y;
  out << YAML::EndMap;
  return out;
}


#endif /* DTOS_H_ */
