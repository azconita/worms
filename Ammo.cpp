/*
 * Ammo.cpp
 *
 *  Created on: Jun 19, 2018
 *      Author: gabi
 */

#include "Ammo.h"

#include "Constants.h"

Ammo::Ammo() {
  this->per_weapon[W_Air_Attack] = Constants::airattack_ammo;
  this->per_weapon[W_Bazooka] = Constants::bazooka_ammo;
  this->per_weapon[Dynamite] = Constants::dynamite_ammo;
  this->per_weapon[Mortar] = Constants::mortar_ammo;
  this->per_weapon[Green_Grenade] = Constants::greengrenade_ammo;
  this->per_weapon[Red_Grenade] = Constants::redgrenade_ammo;
  this->per_weapon[Holy_Grenade] = Constants::holygrenade_ammo;
  this->per_weapon[Teleport] = Constants::teleport_ammo;

  this->per_weapon[Banana] = Constants::banana_ammo;
  this->per_weapon[Baseball_Bat] = Constants::baseballbat_ammo;
}

Ammo::~Ammo() {
  // TODO Auto-generated destructor stub
}

bool Ammo::use(Weapon_Name weapon) {
  if (this->per_weapon.at(weapon) <= 0)
    return false;
  this->per_weapon[weapon] -= 1;
  return true;
}
