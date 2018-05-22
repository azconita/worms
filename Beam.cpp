/*
 * Beam.cpp
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include "Beam.h"
#include <iostream>
#include <vector>

Beam::Beam(b2World* world, float x, float y) : x(x), y(y) {
  //set up static body, store in class variable
  b2BodyDef myBodyDef;
  myBodyDef.type = b2_staticBody;
  myBodyDef.position.Set(x, y);
  this->body = world->CreateBody(&myBodyDef);

  //add box fixture
  b2PolygonShape shape;
  shape.SetAsBox(3, 0.8);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = 1;
  this->body->CreateFixture(&myFixtureDef);
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