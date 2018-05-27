/*
 * Weapon.h
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>
#include "Entity.h"

#ifndef WEAPON_H_
#define WEAPON_H_

#define TYPE_WEAPON 2

class Weapon : public Entity {
private:
protected:
  b2Body* body;

public:

  Weapon(b2World* world, float x, float y);
  virtual ~Weapon();
  int get_type() { return TYPE_WEAPON; }
};

#endif /* WEAPON_H_ */
