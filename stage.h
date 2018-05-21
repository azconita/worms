#include <string>
#include <vector>
#include <Box2D/Box2D.h>
#include "Beam.h"
#include "Worm.h"

#ifndef STAGE_H_
#define STAGE_H_


class Stage {
private:
 std::vector<int> jugadores;
 b2World* world;
 std::vector<Beam> beams;
 std::vector<Worm> worms;

public:
  Stage(std::string config);
  virtual ~Stage();

  void update();
  void make_action(int action);
  //std::vector<b2Vec2 [4]> get_points();
  std::vector<Beam> get_beams();
  std::vector<Worm> get_worms();
private:
  void add_beams(std::string config);
  void add_worms(std::string config);
};

#endif
