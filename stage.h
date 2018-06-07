#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <Box2D/Box2D.h>
#include "StageLoader.h"
#include "Beam.h"
#include "Weapon.h"
#include "Worm.h"
#include "WeaponExplosionListener.h"
#include "Dtos.h"
#include "Logger.h"
#include "Lock.h"


#include "Weapon.h"

#ifndef STAGE_H_
#define STAGE_H_

class Stage {
private:
  std::mutex mutex; 
  //std::vector<int> jugadores;
  b2World* world;
  std::vector<Beam*> beams;
  std::map<int, Worm*> worms;
  std::vector<Weapon*> weapons;

  WeaponExplosionListener explosion_listener;
  std::vector<Weapon*> explosions;

  float wind;

  bool change = false;
  Worm* current_player = NULL;
  time_t player_time = 0;

public:
  Stage(std::string config);
  virtual ~Stage();

  void update();
  void make_action(ActionDTO & action);

  //por ahora...
  StageDTO get_stageDTO();
private:
  void do_explosions();
  void clean_dead_bodies();
  void update_player();
  void change_player();

  std::vector<Beam> get_beams();
  std::vector<Worm> get_worms();
  std::vector<Worm> get_weapons();

  void load_initial_stage(std::string file_name);

  void set_position(ElementDTO & element , std::vector<b2Vec2> & vertices);
};

#endif
