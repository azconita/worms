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

enum Direction {
    Right,
    Left
};

enum State {
   Still,
   Still_up,
   Still_down,
   Walk,
   Fall,
   Walk_up,
   Walk_down,
   Jump_state,
   Jump_up,
   Jump_down,
   Jump_back_state,
   Worm_missile,
   Worm_missile_up,
   Worm_missile_down,
   Worm_banana,
   Worm_banana_up,
   Worm_banana_down,
   Worm_bat,
   Worm_bat_up,
   Worm_bat_down,
   Worm_green_granade,
   Worm_green_granade_up,
   Worm_green_granade_down,
   Worm_red_granade,
   Worm_red_granade_up,
   Worm_red_granade_down,
   Worm_holy_granade,
   Worm_holy_granade_up,
   Worm_holy_granade_down,
   Worm_teleport,
   Worm_teleport_up,
   Worm_teleport_down,
   Worm_disappear,
   Worm_disappear_up,
   Worm_disappear_down,
   Worm_air_attack,
   Worm_air_attack_up,
   Worm_air_attack_down,
   Worm_dynamite_up,
   Worm_dynamite_down,
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
    Big_Explosion,
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
    {Teleport,Worm_teleport},
    {Banana,Worm_banana},
    {Baseball_Bat,Worm_bat}
};

static std::map<State, State> up_states {
   {Fall,Fall},
   {Still,Still_up},
   {Still_up,Still_up},
   {Still_down,Still_up},
   {Walk,Walk_up},
   {Walk_up,Walk_up},
   {Walk_down,Walk_up},
   {Jump_state, Jump_up},
   {Jump_down, Jump_up},
   {Jump_up,Jump_up},
    {Jump_back_state, Jump_back_state},
   {Worm_missile,Worm_missile_up},
   {Worm_missile_up,Worm_missile_up},
   {Worm_missile_down,Worm_missile_up},
   {Worm_banana,Worm_banana_up},
    {Worm_banana_up,Worm_banana_up},
     {Worm_banana_down,Worm_banana_up},
   {Worm_bat,Worm_bat_up},
   {Worm_bat_down,Worm_bat_up},
   {Worm_bat_up,Worm_bat_up},
   {Worm_green_granade,Worm_green_granade_up},
    {Worm_green_granade_up,Worm_green_granade_up},
     {Worm_green_granade_down,Worm_green_granade_up},
   {Worm_red_granade,Worm_red_granade_up},
     {Worm_red_granade_up,Worm_red_granade_up},
       {Worm_red_granade_down,Worm_red_granade_up},
   {Worm_holy_granade,Worm_holy_granade_up},
   {Worm_holy_granade_up,Worm_holy_granade_up},
   {Worm_holy_granade_down,Worm_holy_granade_up},
   {Worm_teleport,Worm_teleport_up},
    {Worm_teleport_up,Worm_teleport_up},
     {Worm_teleport_down,Worm_teleport_up},
   {Worm_disappear,Worm_disappear_up},
   {Worm_disappear_up,Worm_disappear_up},
   {Worm_disappear_down,Worm_disappear_up},
   {Worm_air_attack,Worm_air_attack_up},
    {Worm_air_attack_up,Worm_air_attack_up},
     {Worm_air_attack_down,Worm_air_attack_up},
   {Worm_dynamite,Worm_dynamite_up},
   {Worm_dynamite_up,Worm_dynamite_up},
   {Worm_dynamite_down,Worm_dynamite_up}
};


static std::map<State, State> down_states {
   {Fall,Fall},
   {Still,Still_down},
   {Still_up,Still_down},
   {Still_down,Still_down},
   {Walk,Walk_down},
   {Walk_up,Walk_down},
   {Walk_down,Walk_down},
   {Jump_state, Jump_down},
   {Jump_down, Jump_down},
   {Jump_down,Jump_down},
   {Jump_back_state, Jump_back_state},
   {Worm_missile,Worm_missile_down},
   {Worm_missile_up,Worm_missile_down},
   {Worm_missile_down,Worm_missile_down},
   {Worm_banana,Worm_banana_down},
    {Worm_banana_up,Worm_banana_down},
     {Worm_banana_down,Worm_banana_down},
   {Worm_bat,Worm_bat_down},
   {Worm_bat_down,Worm_bat_down},
   {Worm_bat_up,Worm_bat_down},
   {Worm_green_granade,Worm_green_granade_down},
    {Worm_green_granade_up,Worm_green_granade_down},
     {Worm_green_granade_down,Worm_green_granade_down},
   {Worm_red_granade,Worm_red_granade_down},
     {Worm_red_granade_up,Worm_red_granade_down},
       {Worm_red_granade_down,Worm_red_granade_down},
   {Worm_holy_granade,Worm_holy_granade_down},
   {Worm_holy_granade_up,Worm_holy_granade_down},
   {Worm_holy_granade_down,Worm_holy_granade_down},
   {Worm_teleport,Worm_teleport_down},
    {Worm_teleport_up,Worm_teleport_down},
     {Worm_teleport_down,Worm_teleport_down},
   {Worm_disappear,Worm_disappear_down},
   {Worm_disappear_up,Worm_disappear_down},
   {Worm_disappear_down,Worm_disappear_down},
   {Worm_air_attack,Worm_air_attack_down},
    {Worm_air_attack_up,Worm_air_attack_down},
     {Worm_air_attack_down,Worm_air_attack_down},
   {Worm_dynamite,Worm_dynamite_down},
   {Worm_dynamite_up,Worm_dynamite_down},
   {Worm_dynamite_down,Worm_dynamite_down}
};

struct Vertex{
  float pos_x =0;
  float pos_y =0;
};






struct ElementDTO { //puede ser un gusano, un arma o una viga
  float pos_x = 0;
  float pos_y = 0; //vertice superior izquierdo en metros
  float h = 0;
  float w = 0;
  std::vector<Vertex> vertices;
  State worm_state = Still;
  Direction direction = Right;
  float angle = 0;
  int life = 0;
  int player_id = -2;
  Weapon_Name weapon = None;
  int timer = 0;
  bool explosion = false;
};

struct StageDTO {
  //bool finish = false;
  int worm_turn = -2;
  int player_id = -2;
  int winner = -1;
  std::vector<ElementDTO> beams;
  std::map<int,ElementDTO> worms;
  std::vector<ElementDTO> weapons;
};


enum Movement {
  Not_move,
  Walk_right,
  Walk_left,
  Jump,
  Jump_back
};
enum Action_Type {
  Not_action,
  Make_move,
  Take_weapon,
  Shot_weapon,
  Timer_update,
  Quit
};

struct ActionDTO {
  int player_id = -2;
  int worm_id = -2;
  Action_Type type = Not_action;
  Movement move = Not_move;
  Weapon_Name weapon = None;
  float weapon_degrees = 0;
  int power = 0;
  int time_to_explode = 0;
  Direction direction = Right;
  int pos_x = 0;
  int pos_y = 0;
};



namespace YAML {

template<>
struct convert<StageDTO> {
  static Node encode(const StageDTO& s) {
    Node node;
    node["worm_turn"] = s.worm_turn;
    node["player_id"] = s.player_id;
    node["winner"] = s.winner;
    node["worms"] = s.worms;
    node["beams"] = s.beams;
    node["weapons"] = s.weapons;


    return node;
  }
  static bool decode(const Node& node, StageDTO& s) {
    s.worm_turn = node["worm_turn"].as<int>();
    s.player_id = node["player_id"].as<int>();
    s.winner = node["winner"].as<int>();
    s.beams = node["beams"].as<std::vector<ElementDTO>>();
    s.weapons = node["weapons"].as<std::vector<ElementDTO>>();
    s.worms = node["worms"].as<std::map<int, ElementDTO>>();

    return true;
  }
};


template<>
struct convert<Vertex> {
  static Node encode(const Vertex& v) {
    Node node;
    node["pos_x"] = v.pos_x;
    node["pos_y"] = v.pos_y;
    return node;
  }

  static bool decode(const Node& node, Vertex& v) {
    v.pos_x = node["pos_x"].as<float>();
    v.pos_y = node["pos_y"].as<float>();;
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
    node["vertices"] = elem.vertices;
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
    elem.vertices = node["vertices"].as<std::vector<Vertex>>();
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
    node["player_id"] = action.player_id;
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
    action.player_id = node["player_id"].as<int>();
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
