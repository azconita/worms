/*
 * WeaponExplosionListener.h
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>

#ifndef WEAPONEXPLOSIONLISTENER_H_
#define WEAPONEXPLOSIONLISTENER_H_

class WeaponExplosionListener : public b2ContactListener {
public:
  WeaponExplosionListener();
  virtual ~WeaponExplosionListener();

  void BeginContact(b2Contact* contact);
  void EndContact(b2Contact* contact);
};


#endif /* WEAPONEXPLOSIONLISTENER_H_ */
