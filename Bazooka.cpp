/*
 * Bazooka.cpp
 *  arma con mira
 *  potencia variable
 *  afectada por el viento
 *  infinitas municiones
 *  dano: 50pts(epicentro), 2m (radio)
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#include "Bazooka.h"
#include "Constants.h"
#include "DTOs.h"

Bazooka::Bazooka(b2World* world, float x, float y) :
          Weapon(world, W_Bazooka, x, y) {
  this->damage = Constants::bazooka_damage;
  this->radius = Constants::bazooka_radius;
}

Bazooka::~Bazooka() {
  // TODO Auto-generated destructor stub
}

void Bazooka::shoot(float x, float y, float pot) {
  b2Vec2 vel = body->GetLinearVelocity();
  float velChange = Constants::worm_walk_velocity - vel.x;
  float impulsex = body->GetMass() * velChange;
  velChange = Constants::worm_walk_velocity - vel.y;
  float impulsey = body->GetMass() * velChange;
  this->body->ApplyLinearImpulse(b2Vec2(impulsex,impulsey), this->body->GetWorldCenter(), true);
}
