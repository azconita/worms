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
}

Stage::~Stage() {
  delete this->world;
}

void Stage::update() {
  float32 timeStep = Constants::time_step; //segundos del step
  int32 velocityIterations = Constants::velocity_iterations;   //how strongly to correct velocity
  int32 positionIterations = Constants::position_iterations;   //how strongly to correct position

  this->world->Step( timeStep, velocityIterations, positionIterations);

}

void Stage::make_action(ActionDTO & action) {
  int worm = action.worm_id;
  switch (action.type) {
    case (Make_move):{
      switch(action.move){
        case Walk_right:
          this->worms[worm].move_right();
          break;
        case Walk_left:
          this->worms[worm].move_left();
          break;
        case Jump:
          this->worms[worm].jump();
          break;
        case Jump_back:
          this->worms[worm].jump_back();
          break;
      }
      break;
    }
    case(Take_weapon):{
      printf("Se tomo el arma %i\n", action.weapon);
      break;

    }

    case(Shot_weapon):{
      printf("Se disparo el arma al punto (%i,%i) en metros, con una potencia de %i apuntando a %f grados",//
       action.x, action.y, action.power, action.weapon_degrees);
      break;

    }    
  }
}

void Stage::set_position(ElementDTO & element , std::vector<b2Vec2> & vertices){
    b2Vec2 up_left = vertices[0];
    b2Vec2 down_right = vertices[2];


    element.x = up_left.x;
    element.y = up_left.y;

    if(down_right.y > up_left.y){
      element.h = (down_right.y - up_left.y);
    }else{
      element.h = (up_left.y - down_right.y);
    }
    if(down_right.x > up_left.x){
      element.w = (down_right.x - up_left.x);
    }else{
      element.w = (up_left.x - down_right.x);
    }
}

StageDTO Stage::get_stageDTO() {
  StageDTO s;
  for (auto w: this->worms) {
    ElementDTO worm_element;
    std::vector<b2Vec2> vertices = w.second.get_points();
    set_position(worm_element, vertices);
    //printf("worm %i: x = %f y = %f  h = %f w = %f\n",w.first, worm_element.x, worm_element.y, worm_element.h, worm_element.w);
    s.worms[w.first] = worm_element;
  }

  for (auto b: this->beams) {
    ElementDTO beam_element;
    std::vector<b2Vec2> vertices = b.get_points();
    set_position(beam_element, vertices);
    //printf("beam : x = %f y = %f  h = %f w = %f\n", beam_element.x, beam_element.y, beam_element.h, beam_element.w);
    s.beams.push_back(beam_element);
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
