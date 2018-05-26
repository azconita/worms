#include "stage.h"
#include <string>
#include <vector>
#include <iostream>
#include <Box2D/Box2D.h>
#include "Constants.h"

//config: yaml: https://github.com/jbeder/yaml-cpp/
Stage::Stage(std::string config) {
  std::cout << config << '\n';
  b2Vec2 gravity(0, Constants::gravity); //normal earth gravity

  this->world = new b2World(gravity);
  this->add_beams(config);
  this->add_worms(config);
  this->add_explosion();
}

Stage::~Stage() {
  delete this->world;
}

void Stage::update() {
  float32 timeStep = Constants::time_step; //segundos del step
  int32 velocityIterations = Constants::velocity_iterations;   //how strongly to correct velocity
  int32 positionIterations = Constants::position_iterations;   //how strongly to correct position
  if (this->explosions[0].should_explode()) {
    this->explosions[0].proximityExplosion(1000, 1000);
  }
  worms[0].printPos();
  this->world->Step( timeStep, velocityIterations, positionIterations);

}

void Stage::make_action(int worm, int action) {
  switch (action) {
    case 0:
      this->worms[worm].move_right();
      break;
    case 1:
      this->worms[worm].move_left();
      break;
    case 2:
      this->worms[worm].jump();
      break;
    case 3:
      this->worms[worm].jump_back();
      break;
  }
}

StageDTO Stage::get_stageDTO() {
  StageDTO s;
  for (auto w: this->worms) {
    std::vector<std::tuple<float, float>> v;
    std::vector<b2Vec2> points = w.second.get_points();
    for (auto p: points) {
      v.push_back(std::tuple<float, float>(p.x,p.y));
    }
    s.worms[w.first] = v;
  }
  int i = 0;
  for (auto w: this->beams) {
    std::vector<std::tuple<float, float>> v;
    std::vector<b2Vec2> points = w.get_points();
    for (auto p: points)
      v.push_back(std::tuple<float, float>(p.x,p.y));
    s.beams[i] = v;
    i++;
  }
  s.worm_turn = 0;
  return s;
}



// set initial stage
void Stage::add_beams(std::string config) {
  this->beams.push_back(Beam(this->world, 10,2));
  this->beams.push_back(Beam(this->world, 10,20));
}

void Stage::add_worms(std::string config) {
  Worm w(this->world, 10, 11);
  this->worms.emplace(0, w);
}

void Stage::add_weapons(std::string config) {

}

void Stage::add_explosion() {
  this->explosions.push_back(Explosion(this->world, 11,12));
}
