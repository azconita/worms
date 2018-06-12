/*
 * TurnHelper.h
 *
 *  Created on: Jun 8, 2018
 *      Author: gabi
 */

#ifndef TURNHELPER_H_
#define TURNHELPER_H_

#include <vector>

class TurnHelper {
private:
  std::vector<int> ids; //Worm* ??
  int last;
  int player_id;
public:
  TurnHelper(std::vector<int> worms_ids, int id);
  virtual ~TurnHelper();

  int get_next();
  bool has_worms();
  void delete_id(int id);
};

#endif /* TURNHELPER_H_ */
