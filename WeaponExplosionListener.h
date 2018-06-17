/*
 * WeaponExplosionListener.h
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>

#ifndef WEAPONEXPLOSIONLISTENER_H_
#define WEAPONEXPLOSIONLISTENER_H_

#include <Box2D/Box2D.h>
#include <vector>
#include "Entity.h"
#include "Constants.h"
#include "Weapon.h"
#include "Beam.h"
#include "Worm.h"


#define BEAM_TYPE 0
#define WORM_TYPE 1
#define WEAPON_TYPE 3

class WeaponExplosionListener : public b2ContactListener {
public:
  WeaponExplosionListener();
  virtual ~WeaponExplosionListener();

  void BeginContact(b2Contact* contact);
  void EndContact(b2Contact* contact);
  void handle_begin_explotion(Entity* entityA, Entity* entityB);
  void handle_begin_contact(Entity* entityA, Entity* entityB);
  void handle_end_contact(Entity* entityA, Entity* entityB);

  void beam_worm_begin_contact(Beam * beam, Worm * worm);
  void beam_worm_end_contact(Beam * beam, Worm * worm);
};


#endif /* WEAPONEXPLOSIONLISTENER_H_ */
