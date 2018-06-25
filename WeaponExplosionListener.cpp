/*
 * WeaponExplosionListener.cpp
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */
#include "WeaponExplosionListener.h"
#include <iostream>


WeaponExplosionListener::WeaponExplosionListener() {
  // TODO Auto-generated constructor stub

}

WeaponExplosionListener::~WeaponExplosionListener() {
  // TODO Auto-generated destructor stub
}

void WeaponExplosionListener::BeginContact(b2Contact* contact) {

  Entity* entityA= (Entity*) contact->GetFixtureA()->GetBody()->GetUserData();
  Entity* entityB = (Entity*) contact->GetFixtureB()->GetBody()->GetUserData();
  handle_begin_explotion( entityA, entityB);
  handle_begin_contact( entityA, entityB);


}

void WeaponExplosionListener::EndContact(b2Contact* contact) {
  Entity* entityA= (Entity*) contact->GetFixtureA()->GetBody()->GetUserData();
  Entity* entityB = (Entity*) contact->GetFixtureB()->GetBody()->GetUserData();
  handle_end_contact( entityA, entityB);
}


void WeaponExplosionListener::handle_begin_explotion(Entity* entityA, Entity* entityB){
  if (entityA->en_type == WEAPON_TYPE) {
    static_cast<Weapon*>( entityA)->proximity_explosion(Constants::explosion_power);
  } else if ( entityB->en_type == WEAPON_TYPE ){
    static_cast<Weapon*>( entityB )->proximity_explosion(Constants::explosion_power);
  }
}


void WeaponExplosionListener::handle_begin_contact(Entity* entityA, Entity* entityB){
  if (entityA->en_type == WORM_TYPE && entityB->en_type == BEAM_TYPE){
    Worm* worm = static_cast<Worm*>( entityA);
    Beam * beam = static_cast<Beam*>( entityB);
    this->beam_worm_begin_contact(beam, worm);
  } else if (entityA->en_type == BEAM_TYPE && entityB->en_type ==   WORM_TYPE){
    Beam * beam = static_cast<Beam*>( entityA);
    Worm* worm = static_cast<Worm*>( entityB);
    this->beam_worm_begin_contact(beam, worm);
  }
}

void WeaponExplosionListener::handle_end_contact(Entity* entityA, Entity* entityB){
  if (entityA->en_type == WORM_TYPE && entityB->en_type == BEAM_TYPE){
    Worm* worm = static_cast<Worm*>( entityA);
    Beam * beam = static_cast<Beam*>( entityB);
    this->beam_worm_end_contact(beam, worm);
  } else if (entityA->en_type == BEAM_TYPE && entityB->en_type ==   WORM_TYPE){
    Beam * beam = static_cast<Beam*>( entityA);
    Worm* worm = static_cast<Worm*>( entityB);
    this->beam_worm_end_contact(beam, worm);
  }

}


void WeaponExplosionListener::beam_worm_begin_contact(Beam * beam, Worm * worm){
  float angle = beam->get_angle();
  if(angle == 90){
    printf("[WeaponExplosionListener] rebote con viga de 90 grados\n");
    worm->bounce();
  }
  if(angle > 45 && angle < 135){
    return;
  }
  oLog() << "[WeaponExplosionListener] se inicio el contacto con viga de angulo %f \n", angle;
  std::vector<b2Vec2> beam_pos = beam->get_points();
  worm->set_inclination(angle, beam_pos);
}


void WeaponExplosionListener::beam_worm_end_contact(Beam * beam, Worm * worm){
  //el gusano se encargara solo de saber si se termino el contacto
} 
