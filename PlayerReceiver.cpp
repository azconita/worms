/*
 * PlayerReceiver.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "PlayerReceiver.h"

PlayerReceiver::PlayerReceiver(Socket &client, BlockingQueue<StageDTO> *send_queue,
    BlockingQueue<ActionDTO> *recv_queue) :
    client(client), send_queue(send_queue), recv_queue(recv_queue){
  // TODO Auto-generated constructor stub

}

PlayerReceiver::~PlayerReceiver() {
  // TODO Auto-generated destructor stub
}

void PlayerReceiver::operator ()() {

}
