#include "stage.h"
#include <string>
#include <vector>
#include <iostream>
#include <Box2D/Box2D.h>

//config: yaml: https://github.com/jbeder/yaml-cpp/
Stage::Stage(std::string config) {
  std::cout << config << '\n';
  b2Vec2 gravity(0, -9.8); //normal earth gravity

  this->world = new b2World(gravity);
  this->add_beams(config);
}

Stage::~Stage() {
  delete this->world;
}

void Stage::update() {
  float32 timeStep = 1/20.0; //segundos del step
  int32 velocityIterations = 8;   //how strongly to correct velocity
  int32 positionIterations = 3;   //how strongly to correct position

  this->world->Step( timeStep, velocityIterations, positionIterations);

}

//
void rotateTranslate(b2Vec2& vector,const b2Vec2& center,float angle) {
  b2Vec2 tmp;
  tmp.x=vector.x*cos(angle)-vector.y*sin(angle);
  tmp.y=vector.x*sin(angle)+vector.y*cos(angle);
  vector=tmp+center;
}

std::vector<Beam> Stage::get_beams() {
  return this->beams;
}
std::vector<Worm> Stage::get_worms() {
  return this->worms;
}

// set initial stage
void Stage::add_beams(std::string config) {
  int myints[] = {10,20,30,40};
  std::vector<int> v(myints, myints + sizeof(myints) / sizeof(int) );
  for (auto &g: v) {
    this->beams.push_back(Beam(this->world, g, 20));
  }
}
