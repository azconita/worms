/*
 * TurnHelper.h
 *
 *  Created on: Jun 8, 2018
 *      Author: gabi
 */

#ifndef TURNHELPER_H_
#define TURNHELPER_H_

#include <vector>
#include "Logger.h"

extern  logger oLog;

class TurnHelper {
private:
  std::vector<int> ids; //Worm* ??
  int last;
  int player_id;
public:
  TurnHelper(std::vector<int> worms_ids, int id);
  virtual ~TurnHelper();

  int get_next();
  void delete_id(int id);
  bool is_empty() const;
};

#endif /* TURNHELPER_H_ */
