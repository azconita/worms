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

#include "Constants.h"

Weapon::~Weapon() {
  // TODO Auto-generated destructor stub
}

Weapon::Weapon(b2World* world, float x, float y) {
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
}
