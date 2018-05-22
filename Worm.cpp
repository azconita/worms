/*
 * Worm.cpp
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include "Worm.h"

Worm::Worm(b2World* world, float x, float y) {
  float P2M=1;
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x*P2M, y*P2M);
  this->body = world->CreateBody(&bodyDef);

  //add box fixture
  b2PolygonShape shape;
  shape.SetAsBox(0.5*P2M, 0.5*P2M);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = 1;
  this->body->CreateFixture(&myFixtureDef);
}

Worm::~Worm() {
  // TODO Auto-generated destructor stub
}

void Worm::move_left() {
  this->body->ApplyForce(b2Vec2(-50,0), this->body->GetWorldCenter(), true);
}

void Worm::rotateTranslate(b2Vec2& vector,const b2Vec2& center,float angle) {
  b2Vec2 tmp;
  tmp.x=vector.x*cos(angle)-vector.y*sin(angle);
  tmp.y=vector.x*sin(angle)+vector.y*cos(angle);
  vector=tmp+center;
}

std::vector<b2Vec2> Worm::get_points() {
  std::vector<b2Vec2> points;
  for (int i = 0; i < 4; i++) {
    b2Vec2 p = ((b2PolygonShape*)this->body->GetFixtureList()->GetShape())->GetVertex(i);
    rotateTranslate(p, this->body->GetWorldCenter(), this->body->GetAngle());
    points.push_back(p);
  }

  return points;
}