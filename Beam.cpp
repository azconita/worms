/*
 * Beam.cpp
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include "Beam.h"


Beam::Beam(b2World* world, float size, float x, float y, float angle, Direction direction) : Entity(0) {
  //set up static body, store in class variable
  this->direction = direction;
  if(direction == Left){
    angle = 180 - angle;
  }
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(x, y);
  //bodyDef.angle = angle* (M_PI / 180); no poner esto porque funciona mal
   this->angle = angle;
  this->body = world->CreateBody(&bodyDef);
  
  //add box fixture
  b2PolygonShape shape;
  shape.SetAsBox((size+0.0)/2 -0.5, (Constants::beam_height+0.0)/2,b2Vec2(0,0),angle* (M_PI / 180));
  b2FixtureDef fixture;
  fixture.shape = &shape;
  fixture.friction = Constants::beam_friction;
  fixture.density = 100;
  this->body->CreateFixture(&fixture);
  this->body->SetUserData(this);

}

Beam::Beam(const Beam &other) : Entity(0), body(other.body) {
  //this->body->SetUserData(this);
}

Beam* Beam::operator=(const Beam &other) {
  this->body = other.body;
  this->angle = other.angle;
  this->body->SetUserData(this);
  return this;
}

Beam::~Beam() {
	// TODO Auto-generated destructor stub
}

std::vector<b2Vec2> Beam::get_points() {
  std::vector<b2Vec2> points;
  for (int i = 0; i < 4; i++)
    points.push_back(((b2PolygonShape*)this->body->GetFixtureList()->GetShape())->GetVertex(i) + this->body->GetWorldCenter());
  return points;
}

float Beam::get_angle() {
  return this->angle;
}

Direction Beam::get_direction() {
  return this->direction;
}

b2Vec2 Beam::get_center(){
  std::vector<b2Vec2> v=  this->get_points();
  return this->body->GetPosition();
}
