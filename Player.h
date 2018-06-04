/*
 * Player.h
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "common_socket.h"

class Player {
  Socket &client;
public:
  Player(Socket &client);
  virtual ~Player();
};

#endif /* PLAYER_H_ */
