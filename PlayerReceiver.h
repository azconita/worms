/*
 * PlayerReceiver.h
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#ifndef PLAYERRECEIVER_H_
#define PLAYERRECEIVER_H_

#include "BlockingQueue.h"
#include "DTOs.h"
#include "common_socket.h"

class PlayerReceiver {
private:
  Socket &client;
  BlockingQueue<ActionDTO> *recv_queue;

  bool finished = false;
public:
  PlayerReceiver(Socket &client, BlockingQueue<ActionDTO> *recv_queue);
  virtual ~PlayerReceiver();

  void operator()();

private:
  ActionDTO get_dto(std::string str);
};

#endif /* PLAYERRECEIVER_H_ */
