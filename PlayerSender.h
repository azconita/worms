/*
 * PlayerSender.h
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#ifndef PLAYERSENDER_H_
#define PLAYERSENDER_H_

#include "BlockingQueue.h"
#include "DTOs.h"
#include "common_socket.h"

class PlayerSender {
private:
  Socket &client;
  BlockingQueue<StageDTO> *send_queue;
  BlockingQueue<ActionDTO> *recv_queue;
public:
  PlayerSender(Socket &client, BlockingQueue<StageDTO> *send_queue,
      BlockingQueue<ActionDTO> *recv_queue);
  virtual ~PlayerSender();

  void operator()();
};

#endif /* PLAYERSENDER_H_ */
