/*
 * Player.h
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <thread>
#include "Socket.h"
#include "BlockingQueue.h"
#include "Dtos.h"

class Player {
  Socket &client;
  int id = -1;
  BlockingQueue<StageDTO> *send_queue;
  BlockingQueue<ActionDTO> *recv_queue;
  std::thread sender;
  std::thread receiver;
  bool on;
public:
  Player(Socket &client);
  virtual ~Player();
  void set_id(int id);
  void add_stage_queues(BlockingQueue<StageDTO> *send_queue,
                        BlockingQueue<ActionDTO> *recv_queue);
  void start();

  void send();
  void receive();
  void stop();
  void end_game(int winner);
};

#endif /* PLAYER_H_ */
