/*
 * Ammo.h
 *
 *  Created on: Jun 19, 2018
 *      Author: gabi
 */

#ifndef AMMO_H_
#define AMMO_H_

#include <map>
#include "Dtos.h"
#include "Logger.h"

extern  logger oLog;

class Ammo {
  std::map<Weapon_Name, int> per_weapon;
public:
  Ammo();
  virtual ~Ammo();
  bool use(Weapon_Name weapon);
};

#endif /* AMMO_H_ */
