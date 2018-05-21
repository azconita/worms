/*
 * Worm.cpp
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include "Worm.h"

Worm::Worm(b2World* world, float x, float y) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);
  this->body = world->CreateBody(&bodyDef);

  //add box fixture
  b2PolygonShape shape;
  shape.SetAsBox(0.7, 0.7, b2Vec2(x,y), 0.0);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = 1;
  this->body->CreateFixture(&myFixtureDef);
}

Worm::~Worm() {
  // TODO Auto-generated destructor stub
}

std::vector<b2Vec2> Worm::get_points() {
  std::vector<b2Vec2> points;
  for (int i = 0; i < 4; i++)
    points.push_back(((b2PolygonShape*)this->body->GetFixtureList()->GetShape())->GetVertex(i));
  return points;
}
