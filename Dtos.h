/*
 * DTOs.h
 *
 *  Created on: May 27, 2018
 *      Author: gabi
 */

#ifndef DTOS_H_
#define DTOS_H_

#include "yaml-cpp/yaml.h"
#include "Logger.h"
#include <map>
#include <vector>
#include <string>




enum Direction{
    Right,
    Left
};

enum State{
   Still,
   Walk,
   Fall,
   Jump_state,
   Jump_back_state,
   Worm_missile,
   Worm_banana,
   Worm_bat,
   Worm_green_granade,
   Worm_red_granade,
   Worm_holy_granade,
   Worm_teletrans,
   Worm_air_attack,
   Worm_dynamite
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

static std::map<Weapon_Name, State> weapons_states {
    {W_Air_Attack, Worm_air_attack},
    {W_Bazooka,Worm_missile},
    {Dynamite,Worm_dynamite},
    {Mortar,Worm_missile},
    {Green_Grenade,Worm_green_granade},
    {Holy_Grenade,Worm_holy_granade},
    {Red_Grenade,Worm_red_granade},
    {Teleport,Worm_teletrans},
    {Banana,Worm_banana},
    {Baseball_Bat,Worm_bat}
};

struct ElementDTO { //puede ser un gusano, un arma o una viga
  float pos_x;
  float pos_y; //vertice superior izquierdo en metros
  float h;
  float w;
  State worm_state;
  Direction direction;
  float angle;
  int life;
  int player_id;
  Weapon_Name weapon;
  int timer = 0;
  bool explosion;
};

struct StageDTO {
  //bool finish = false;
  int worm_turn;
  int player_id;
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
  Shot_weapon,
  Timer_update,
  Quit
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
    node["player_id"] = s.player_id;
    node["worms"] = s.worms;
    node["beams"] = s.beams;
    node["weapons"] = s.weapons;


    return node;
  }
  static bool decode(const Node& node, StageDTO& s) {
    s.worm_turn = node["worm_turn"].as<int>();
    s.player_id = node["player_id"].as<int>();
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
    node["worm_state"] = (int)elem.worm_state;
    node["direction"] = (int)elem.direction;
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
    elem.worm_state = static_cast<State>(node["worm_state"].as<int>());
    elem.direction = static_cast<Direction>(node["direction"].as<int>());
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


YAML::Emitter& operator<<(YAML::Emitter& out, const ElementDTO& e);

YAML::Emitter& operator << (YAML::Emitter& out, const StageDTO& s);

YAML::Emitter& operator << (YAML::Emitter& out, const ActionDTO& a);

#endif /* DTOS_H_ */
