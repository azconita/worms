/*
 * PlayerSender.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "PlayerSender.h"

PlayerSender::PlayerSender(Socket &client, BlockingQueue<StageDTO> *send_queue,
    BlockingQueue<ActionDTO> *recv_queue) :
    client(client), send_queue(send_queue), recv_queue(recv_queue) {

}

PlayerSender::~PlayerSender() {
  // TODO Auto-generated destructor stub
}


void PlayerSender::operator()() {

}
