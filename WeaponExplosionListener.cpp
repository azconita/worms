/*
 * WeaponExplosionListener.cpp
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */
#include <Box2D/Box2D.h>
#include "Entity.h"
#include <iostream>

#include "Projectile.h"
#include "WeaponExplosionListener.h"

WeaponExplosionListener::WeaponExplosionListener() {
  // TODO Auto-generated constructor stub

}

WeaponExplosionListener::~WeaponExplosionListener() {
  // TODO Auto-generated destructor stub
}

void WeaponExplosionListener::BeginContact(b2Contact* contact) {

  //check if fixture A was a explsion
  Entity* bodyUserData = (Entity*) contact->GetFixtureA()->GetBody()->GetUserData();
  std::cout << "entity read " << bodyUserData<< "\n";
  if (bodyUserData->en_type == 3) {
    static_cast<Projectile*>( bodyUserData )->proximityExplosion(100,100);

  } else {
  //check if fixture B was a explosion
    bodyUserData = (Entity*) contact->GetFixtureB()->GetBody()->GetUserData();
    std::cout << "entity read " << bodyUserData<< "\n";
    if ( bodyUserData->en_type == 3 )
      static_cast<Projectile*>( bodyUserData )->proximityExplosion(100,100);
  }

}

void WeaponExplosionListener::EndContact(b2Contact* contact) {

  //check if fixture A was a ball
  void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
  if ( bodyUserData )
    return;
  //check if fixture B was a ball
  bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
  if ( bodyUserData )
    return;
}
