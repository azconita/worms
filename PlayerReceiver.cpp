/*
 * PlayerReceiver.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "PlayerReceiver.h"

PlayerReceiver::PlayerReceiver(Socket &client, BlockingQueue<ActionDTO> *recv_queue) :
    client(client), recv_queue(recv_queue){
  // TODO Auto-generated constructor stub

}

PlayerReceiver::~PlayerReceiver() {
  // TODO Auto-generated destructor stub
}

void PlayerReceiver::operator ()() {
  while (!this->finished) {
    std::string str = this->client.recv_string();
    ActionDTO dto = this->get_dto(str);
    this->recv_queue(dto);
  }
}

//TODO que use lo de jaz!
ActionDTO PlayerReceiver::get_dto(std::string str) {
  ActionDTO dto;
  dto.type = Timer_update;
  return dto;
}
