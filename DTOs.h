/*
 * DTOs.h
 *
 *  Created on: May 27, 2018
 *      Author: gabi
 */

#ifndef DTOS_H_
#define DTOS_H_

#include <map>
#include <vector>
#include <string>

#include <yaml-cpp/yaml.h>

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


static  std::map<int,Weapon_Name> get_weapon_name{
    {0,W_Air_Attack}, 
    {1,W_Bazooka}, 
    {2,Dynamite}, 
    {3,Mortar}, 
    {4,W_Fragment},
    {5,Green_Grenade},
    {6,Holy_Grenade},
    {7,Red_Grenade},
    {8,Teleport},
    {9,Banana},
    {10,Baseball_Bat},
    {11,Explosion},
    {12,None},
    {13,W_Timer}
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
  int x;
  int y;
};



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

YAML::Emitter& operator << (YAML::Emitter& out, const ElementDTO& e);

struct StageDTO {
  int worm_turn;
  std::map<int,ElementDTO> worms;
  std::vector<ElementDTO> beams;
  std::vector<ElementDTO> weapons;
};

YAML::Emitter& operator << (YAML::Emitter& out, const StageDTO& s);

namespace YAML {

template<>
struct convert<StageDTO> {
  static Node encode(const StageDTO& s) {
    Node node;
    node["worms"] = s.worms;
    node["beams"] = s.beams;
    node["weapons"] = s.weapons;
 
  
    return node;
  }
  static bool decode(const Node& node, StageDTO& s) {
    s.worms = node["worms"].as<std::map<int, ElementDTO>>();
    s.beams = node["beams"].as<std::vector<ElementDTO>>();
    s.weapons = node["weapons"].as<std::vector<ElementDTO>>();

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
    node["weapon"] = elem.weapon;
    node["timer"] = elem.timer;
    node["explosion"] = elem.explosion;
    return node;
  }

  static bool decode(const Node& node, ElementDTO& elem) {
    if(!node.IsSequence() || node.size() != 9) {
      return false;
    }
    elem.pos_x = node["pos_x"].as<float>();
    elem.pos_y = node["pos_y"].as<float>();
    elem.h = node["h"].as<float>();
    elem.w = node["w"].as<float>();
    elem.life = node["life"].as<int>();
    elem.player_id = node["player_id"].as<int>();
    //hacer encode/decode de Weapon_Name
    elem.weapon = node["weapon"].as<int>();
    elem.timer = node["timer"].as<int>();
    elem.explosion = node["explosion"].as<bool>();
    return true;
  }
};


}

#endif /* DTOS_H_ */
