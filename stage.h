#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <Box2D/Box2D.h>
#include "Beam.h"
#include "Worm.h"

#ifndef STAGE_H_
#define STAGE_H_

struct StageDTO {
  std::map<int,std::vector<std::tuple<float, float>>> worms;
  std::map<int,std::vector<std::tuple<float, float>>> beams;
};

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
  StageDTO get_positions();
private:
  std::vector<Beam> get_beams();
  std::vector<Worm> get_worms();

  void add_beams(std::string config);
  void add_worms(std::string config);
};

#endif
