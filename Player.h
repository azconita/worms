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
  BlockingQueue<StageDTO> *send_queue;
  BlockingQueue<ActionDTO> *recv_queue;
public:
  Player(Socket &client);
  virtual ~Player();

  void add_stage_queues(BlockingQueue<StageDTO> *send_queue,
                        BlockingQueue<ActionDTO> *recv_queue);
};

#endif /* PLAYER_H_ */
