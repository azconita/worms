#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <random>
#include <Box2D/Box2D.h>
#include "StageLoader.h"
#include "Beam.h"
#include "Weapon.h"
#include "Constants.h"
#include "Worm.h"
#include "WeaponExplosionListener.h"
#include "Dtos.h"
#include "Logger.h"
#include "TurnHelper.h"
#include "Error.h"
#include "Weapon.h"

#ifndef STAGE_H_
#define STAGE_H_

extern logger oLog;

class Stage {
private:
  //std::vector<int> jugadores;
  b2World* world;
  std::vector<Beam*> beams;
  std::map<int, Worm*> worms;
  std::vector<Weapon*> weapons;

  WeaponExplosionListener explosion_listener;
  std::vector<Weapon*> explosions;

  float wind;

  std::map<int, TurnHelper> players_turn;
  int last_player_id = 0;
  bool change = false;
  Worm* current_player = NULL;
  time_t player_time = time(NULL);

  bool finish = false;

public:
  Stage(std::string config);
  virtual ~Stage();

  void update();
  void make_action(ActionDTO & action);

  bool finished();
  void end();

  //por ahora...
  StageDTO get_stageDTO();
  void set_worms_to_players(int total_players);
private:
  void do_explosions();
  void clean_dead_bodies();
  void update_player();
  void change_player();

  void update_worms();


  void load_initial_stage(std::string file_name);

  void set_position(ElementDTO & element ,b2Vec2 & center);
};

#endif
