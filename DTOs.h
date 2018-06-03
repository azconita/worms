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
  float x;
  float y; //vertice superior izquierdo en metros
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
  std::map<int,ElementDTO> worms;
  std::vector<ElementDTO> beams;
  std::vector<ElementDTO> weapons;
};



#endif /* DTOS_H_ */
