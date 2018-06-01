/*
 * WeaponExplosionListener.cpp
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */
#include <Box2D/Box2D.h>
#include "Entity.h"
#include <iostream>

#include "WeaponExplosionListener.h"
#include "Weapon.h"

WeaponExplosionListener::WeaponExplosionListener() {
  // TODO Auto-generated constructor stub

}

WeaponExplosionListener::~WeaponExplosionListener() {
  // TODO Auto-generated destructor stub
}

void WeaponExplosionListener::BeginContact(b2Contact* contact) {

  //check if fixture A was a explosion
  Entity* bodyUserData = (Entity*) contact->GetFixtureA()->GetBody()->GetUserData();
  std::cout << "entity read " << bodyUserData<< "\n";
  if (bodyUserData->en_type == 3) {
    static_cast<Weapon*>( bodyUserData )->proximity_explosion(10,10);

  } else {
  //check if fixture B was a explosion
    bodyUserData = (Entity*) contact->GetFixtureB()->GetBody()->GetUserData();
    //std::cout << "entity read " << bodyUserData<< "\n";
    if ( bodyUserData->en_type == 3 )
      static_cast<Weapon*>( bodyUserData )->proximity_explosion(10,10);
  }

}

void WeaponExplosionListener::EndContact(b2Contact* contact) {

  //check if fixture A was a ball
  Entity* bodyUserData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData();
  if ( bodyUserData->en_type == 3 )
    return;
  //check if fixture B was a ball
  bodyUserData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData();
  if ( bodyUserData )
    return;
}
