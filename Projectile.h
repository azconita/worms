/*
 * Explosion.h
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#include "Entity.h"
#include <vector>
#include <iostream>
#include <Box2D/Box2D.h>
#include "DTOs.h"

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#define PROJECTILE 3

class Projectile : public Entity {
private:
  b2World *world;
  b2Body* body;
  Weapon_Name name;
  int i = 100;
public:
  Projectile(b2World *world, Weapon_Name name, float x, float y);
  Projectile(const Projectile &other);
  virtual ~Projectile();
  Projectile* operator=(const Projectile &other);
  int get_type() { return PROJECTILE; }
  bool should_explode();
  void proximity_explosion(float blastRadius, float blastPower);
  void shoot(int power, float degrees);

  b2Vec2 get_point();


  void printPos() {
      std::cout << "explosion: " << this->body->GetPosition().x << ":" << this->body->GetPosition().y << "\n";
    }
};

//subclass b2QueryCallback
class ExplosionQueryCallback : public b2QueryCallback {
public:
    std::vector<b2Body*> foundBodies;

    bool ReportFixture(b2Fixture* fixture) {
        foundBodies.push_back( fixture->GetBody() );
        return true;//keep going to find all fixtures in the query area
    }
};

#endif /* PROJECTILE_H_ */
