/*
 * TurnHelper.h
 *
 *  Created on: Jun 8, 2018
 *      Author: gabi
 */

#ifndef PLAYERTURN_H_
#define PLAYERTURN_H_

#include <vector>

class PlayerTurn {
private:
  std::vector<int> worms_ids; //Worm* ??
  int last;
  int player_id;
public:
  PlayerTurn(std::vector<int> worms_ids, int id);
  virtual ~PlayerTurn();
  int get_worms_size();

  int get_next_worm_id();
  void delete_worm_id(int id);
};

#endif /* PLAYERTURN_H_ */
