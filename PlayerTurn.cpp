/*
 * TurnHelper.cpp
 *
 *  Created on: Jun 8, 2018
 *      Author: gabi
 */

#include "PlayerTurn.h"

#include <algorithm>
#include <cstdio>

PlayerTurn::PlayerTurn(std::vector<int> worms_ids, int id) :
            worms_ids(worms_ids),
            last(0), player_id(id) {
  printf("[PlayerTurn]cantidad de gusanos en el turn helper: %d, id: %d\n", this->worms_ids.size(), this->player_id);
}

int PlayerTurn::get_next_worm_id() {
  this->last = ((this->last + 1) >= this->worms_ids.size()) ? 0 : this->last + 1;
  return this->worms_ids.at(this->last);
}

void PlayerTurn::delete_worm_id(int id) {
 if (this->worms_ids.at(this->last) == id)
   this->get_next_worm_id();
 this->worms_ids.erase(std::remove(this->worms_ids.begin(), this->worms_ids.end(), id), this->worms_ids.end());
 printf("[PlayerTurn]total worms: %i\n", this->worms_ids.size());
}


PlayerTurn::~PlayerTurn() {
  // TODO Auto-generated destructor stub
}
