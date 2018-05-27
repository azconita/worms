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

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#define EXPLOSION 3

class Projectile : public Entity {
private:
  b2World *world;
  b2Body* body;
  int i = 100;
public:
  Projectile(b2World *world, float x, float y);
  Projectile(const Projectile &other);
  virtual ~Projectile();
  Projectile* operator=(const Projectile &other);
  int get_type() { return EXPLOSION; }
  bool should_explode();
  void proximityExplosion(float blastRadius, float blastPower);

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
