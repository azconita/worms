/*
 * PlayerSender.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "PlayerSender.h"

PlayerSender::PlayerSender(Socket &client, BlockingQueue<StageDTO> *send_queue) :
    client(client), send_queue(send_queue) {

}

PlayerSender::~PlayerSender() {
  // TODO Auto-generated destructor stub
}


void PlayerSender::operator()() {
  while (!this->finished) {
    StageDTO s = this->send_queue->pop();
    if (s.worm_turn == -1) {
      //end game
      this->finished = true;
    } else {
      std::string str = this->get_yaml(s);
      this->client.send_string(str);
    }
  }
}

std::string PlayerSender::get_yaml(StageDTO &s) {

  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "stage";
  out << YAML::Value << s;
  out << YAML::EndMap;

  return out.c_str();
}
