/*
 * Weapon.h
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>
#include "Entity.h"
#include "DTOs.h"

#ifndef WEAPON_H_
#define WEAPON_H_

#define TYPE_WEAPON 2

class Weapon : public Entity {
private:
  Weapon_Name name = None;
protected:
  b2Body* body;

public:

  Weapon(b2World* world, Weapon_Name name, float x, float y);
  virtual ~Weapon();
  int get_type() { return E_Weapon; }

  void shoot(int power, float degrees);
};

#endif /* WEAPON_H_ */
