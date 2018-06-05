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
  BlockingQueue<StageDTO> *send_queue;
  BlockingQueue<ActionDTO> *recv_queue;
public:
  PlayerReceiver(Socket &client, BlockingQueue<StageDTO> *send_queue,
      BlockingQueue<ActionDTO> *recv_queue);
  virtual ~PlayerReceiver();

  void operator()();
};

#endif /* PLAYERRECEIVER_H_ */
