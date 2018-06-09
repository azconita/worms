/*
 * TurnHelper.cpp
 *
 *  Created on: Jun 8, 2018
 *      Author: gabi
 */

#include "TurnHelper.h"

#include <algorithm>
#include <cstdio>

TurnHelper::TurnHelper(std::vector<int> worms_ids, int id) :
            ids(worms_ids),
            last(0), player_id(id) {
  printf("cantidad de gusanos en el turn helper: %d", this->ids.size());
}

TurnHelper::~TurnHelper() {
  // TODO Auto-generated destructor stub
}

int TurnHelper::get_next() {
  this->last = (++(this->last) == this->ids.size()) ? 0 : this->last;
  return this->last;
}

void TurnHelper::delete_id(int id) {
 if (this->ids[this->last] == id)
   this->last = this->get_next();
 this->ids.erase(std::remove(this->ids.begin(), this->ids.end(), id), this->ids.end());
}