/*
 * Explosion.h
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

class Explosion {
private:
  b2World *world;
  b2Body* body;
  int i = 100;
public:
  Explosion(b2World *world, float x, float y);
  virtual ~Explosion();
  bool should_explode();
  void proximityExplosion(float blastRadius, float blastPower);
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

#endif /* EXPLOSION_H_ */
