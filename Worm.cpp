/*
 * Worm.cpp
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include "Worm.h"
#include "Constants.h"
#include <string>

Worm::Worm(b2World* world, float x, float y) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);
  bodyDef.userData = (void*) "worm";
  this->body = world->CreateBody(&bodyDef);

  //add box fixture
  b2PolygonShape shape;
  shape.SetAsBox(Constants::worm_size, Constants::worm_size);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = Constants::worm_density;
  this->body->CreateFixture(&myFixtureDef);
  this->life = Constants::worm_initial_life;
}

Worm::Worm(const Worm& other) :  body(other.body), life(other.life) {
}

Worm::Worm() : body(NULL), life(0) {

}

Worm::~Worm() {
  // TODO Auto-generated destructor stub
}

float Worm::get_impulse() {
  b2Vec2 vel = body->GetLinearVelocity();
  float velChange = Constants::worm_walk_velocity - vel.x;
  return body->GetMass() * velChange;
}

void Worm::move_right() {
  float impulse = this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(impulse,0), this->body->GetWorldCenter(), true);
}

void Worm::move_left() {
  float impulse = this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(-impulse,0), this->body->GetWorldCenter(), true);
}

//TODO: fix me!!
void Worm::jump() {
  float impulse = body->GetMass() * 1.5;
  this->body->ApplyLinearImpulse(b2Vec2(this->get_impulse(),-impulse), this->body->GetWorldCenter(), true);
}
//TODO: fix me!!
void Worm::jump_back() {
  float impulse = this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(-impulse,impulse), this->body->GetWorldCenter(), true);
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
