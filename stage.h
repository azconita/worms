#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <Box2D/Box2D.h>
#include "Beam.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Worm.h"
#include "WeaponExplosionListener.h"
#include "DTOs.h"

#include "Bazooka.h"

#ifndef STAGE_H_
#define STAGE_H_

class Stage {
private:
  std::vector<int> jugadores;
  b2World* world;
  std::vector<Beam*> beams;
  std::map<int, Worm*> worms;
  std::vector<Weapon*> weapons;

  WeaponExplosionListener explosion_listener;
  std::vector<Projectile*> explosions;

  float wind;

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
  void add_explosion();

  void set_position(ElementDTO & element , std::vector<b2Vec2> & vertices);
};

#endif
