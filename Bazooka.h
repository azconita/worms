/*
 * Bazooka.h
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#ifndef BAZOOKA_H_
#define BAZOOKA_H_

#include "Weapon.h"

class Bazooka: public Weapon {
  //int ammo = 10000; //infinita
  int damage;
  int radius;
public:
  Bazooka(b2World* world, float x, float y);
  virtual ~Bazooka();
  void shoot(float x, float y, float pot);
};

#endif /* BAZOOKA_H_ */
