/*
 * Weapon.cpp
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#include "Weapon.h"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>
#include <iostream>

#include "Constants.h"

Weapon::~Weapon() {
  // TODO Auto-generated destructor stub
}

Weapon::Weapon(b2World* world, Weapon_Name name, float x, float y)
                                           : Entity(2) , name(name) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.bullet = true;
  bodyDef.position.Set(x, y);
  this->body = world->CreateBody(&bodyDef);


  //add box fixture
  b2CircleShape shape;
  shape.m_radius = Constants::weapon_size;
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = Constants::weapon_density;
  this->body->CreateFixture(&myFixtureDef);
  this->body->SetUserData(this);
}


void Weapon::shoot(int power, float degrees) {
  std::cout << "SHOOT!\n";
  float r = degrees * (3.14159265359/180.0);
  b2Vec2 vel = b2Vec2(cos(r), sin(r));
  float velChange = power * vel.x;
  float impulsex = body->GetMass() * velChange;
  velChange = power * vel.y;
  float impulsey = body->GetMass() * velChange;
  this->body->ApplyLinearImpulse(b2Vec2(impulsex,impulsey), this->body->GetWorldCenter(), true);
}
