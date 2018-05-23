/*
 * Weapon.h
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>

#ifndef WEAPON_H_
#define WEAPON_H_

class Weapon {
private:
  b2Body* body;

public:

  Weapon(b2World* world, float x, float y);
  virtual ~Weapon();
};

#endif /* WEAPON_H_ */
