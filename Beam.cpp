/*
 * Beam.cpp
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include "Beam.h"
#include <iostream>
#include <vector>
#include "Constants.h"
#include "Logger.h"

Beam::Beam(b2World* world, float size, float x, float y, float angle) : Entity(0) {
  //set up static body, store in class variable
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(x, y);
  bodyDef.angle = angle;
  //myBodyDef.userData = (void*) this;
  this->body = world->CreateBody(&bodyDef);
  //add box fixture
  b2PolygonShape shape;
  extern  logger oLog; 
  oLog() << "beam wight" << size;
  shape.SetAsBox((size+0.0)/2, Constants::beam_height /2);//, b2Vec2(x, y), angle);
  b2FixtureDef fixture;
  fixture.shape = &shape;
  fixture.friction = Constants::beam_friction;
  fixture.density = 1;
  this->body->CreateFixture(&fixture);
  this->body->SetUserData(this);
}

Beam::Beam(const Beam &other) : Entity(0), body(other.body) {
  //this->body->SetUserData(this);
}

Beam* Beam::operator=(const Beam &other) {
  this->body = other.body;
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
  return this->body->GetAngle();
}

b2Vec2 Beam::get_center(){
  return this->body->GetPosition();
}
