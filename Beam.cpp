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

Beam::Beam(b2World* world, float x, float y) : Entity(0) {
  //set up static body, store in class variable
  b2BodyDef myBodyDef;
  myBodyDef.type = b2_staticBody;
  myBodyDef.position.Set(x, y);
  //myBodyDef.userData = (void*) this;
  this->body = world->CreateBody(&myBodyDef);
  std::cout << "beamDir: " << this << '\n';
  //add box fixture
  b2PolygonShape shape;
  shape.SetAsBox(Constants::beam_width/2, Constants::beam_height/2);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = 1;
  this->body->CreateFixture(&myFixtureDef);
  this->body->SetUserData(this);
}

Beam::Beam(const Beam &other) : Entity(0), body(other.body) {
  //this->body->SetUserData(this);
  std::cout << "beamDir(&other): " << this << '\n';
}

Beam* Beam::operator=(const Beam &other) {
  std::cout << "beamDir=: " << this << '\n';
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
