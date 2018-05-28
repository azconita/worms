#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <Box2D/Box2D.h>
#include "Beam.h"
#include "Weapon.h"
#include "Worm.h"

#ifndef STAGE_H_
#define STAGE_H_

enum Weapon_Name{
    Air_Attack, //a
    Bazooka, //b
    Dynamite, //d
    Mortar, //m
    Green_Granade,//g
    Holy_Granade,//h
    Red_Granade,//r
    Teletrans,//t
    Banana,//u
    Baseboll_Bat,//v
    Explosion
};

enum Movement{
  Walk_right,
  Walk_left,
  Jump,
  Jump_back
};
enum Action_Type{
  Make_move,
  Take_weapon,
  Shot_weapon
};

struct ActionDTO{
  int worm_id;
  Action_Type type;
  Movement move;
  Weapon_Name weapon;
  float weapon_degrees;
  int power;
  int x;
  int y;
};



struct ElementDTO{ //puede ser un gusano, un arma o una viga  
  float x; 
  float y; //vertice superior izquierdo en metros
  float h;
  float w;
  int life;
  int player_id;
  Weapon_Name weapon;
  bool explosion;
};

struct StageDTO {
  int worm_turn;
  std::map<int,ElementDTO> worms;
  std::vector<ElementDTO> beams;
  std::vector<ElementDTO> weapons;
};


class Stage {
private:
 std::vector<int> jugadores;
 b2World* world;
 std::vector<Beam> beams;
 std::map<int, Worm> worms;
 std::vector<Weapon> weapons;

public:
  Stage(std::string config);
  virtual ~Stage();

  void update();
  void make_action(ActionDTO & action);

  //por ahora...
  StageDTO get_stageDTO();
private:
  std::vector<Beam> get_beams();
  std::vector<Worm> get_worms();
  std::vector<Worm> get_weapons();

  void add_beams(std::string config);
  void add_worms(std::string config);
  void add_weapons(std::string config);
  void set_position(ElementDTO & element , std::vector<b2Vec2> & vertices);
};

#endif
