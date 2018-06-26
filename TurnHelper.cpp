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
}

TurnHelper::~TurnHelper() {
}

int TurnHelper::get_next() {
  this->last = ((this->last + 1) >= this->ids.size()) ? 0 : this->last + 1;
  return this->ids.at(this->last);
}

void TurnHelper::delete_id(int id) {
 if (this->ids.at(this->last) == id)
   	this->get_next();
 	this->ids.erase(std::remove(this->ids.begin(), this->ids.end(), id), this->ids.end());
}

bool TurnHelper::is_empty() const {
	return (this->ids.size() < 1);
}
