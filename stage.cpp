#include "stage.h"
#include <string>
#include <vector>
#include <iostream>
#include <Box2D/Box2D.h>

//config: yaml: https://github.com/jbeder/yaml-cpp/
Stage::Stage(std::string config) {
  std::cout << config << '\n';
  b2Vec2 gravity(0, 9.8); //normal earth gravity

  this->world = new b2World(gravity);
  this->add_beams(config);
  this->add_worms(config);
}

Stage::~Stage() {
  delete this->world;
}

void Stage::update() {
  float32 timeStep = 0.05; //segundos del step
  int32 velocityIterations = 8;   //how strongly to correct velocity
  int32 positionIterations = 3;   //how strongly to correct position

  this->world->Step( timeStep, velocityIterations, positionIterations);

}

void Stage::make_action(int action) {
  this->worms.front().move_left();
}

StageDTO Stage::get_positions() {
  StageDTO s;
  int i = 0;
  for (auto w: this->worms) {
    std::vector<std::tuple<float, float>> v;
    std::vector<b2Vec2> points = w.get_points();
    for (auto p: points)
      v.push_back(std::tuple<float, float>(p.x,p.y));
    s.worms[i] = v;
    i++;
  }
  i = 0;
  for (auto w: this->beams) {
    std::vector<std::tuple<float, float>> v;
    std::vector<b2Vec2> points = w.get_points();
    for (auto p: points)
      v.push_back(std::tuple<float, float>(p.x,p.y));
    s.beams[i] = v;
    i++;
  }
  return s;
}

std::vector<Beam> Stage::get_beams() {
  return this->beams;
}
std::vector<Worm> Stage::get_worms() {
  return this->worms;
}

// set initial stage
void Stage::add_beams(std::string config) {
  const int WIDTH=640;
  const int HEIGHT=480;
  const float M2P=20;
  const float P2M=1/M2P;

  int myints[] = {-10,-20};
  std::vector<int> v(myints, myints + sizeof(myints) / sizeof(int) );
  this->beams.push_back(Beam(this->world, 10,0));
  this->beams.push_back(Beam(this->world, 10,20));
  /*for (auto &g: v) {
    this->beams.push_back(Beam(this->world, 20, g));
  }*/
}

void Stage::add_worms(std::string config) {
  this->worms.push_back(Worm(this->world, 11, 10));
}