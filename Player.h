/*
 * Player.h
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <thread>
#include "common_socket.h"
#include "BlockingQueue.h"
#include "DTOs.h"

class Player {
  Socket &client;
  BlockingQueue<StageDTO> *send_queue;
  BlockingQueue<ActionDTO> *recv_queue;
  std::thread sender;
  std::thread receiver;
public:
  Player(Socket &client);
  virtual ~Player();

  void add_stage_queues(BlockingQueue<StageDTO> *send_queue,
                        BlockingQueue<ActionDTO> *recv_queue);
  void start();
};

#endif /* PLAYER_H_ */
