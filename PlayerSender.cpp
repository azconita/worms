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
  /*Stage stage("file.yaml");
    Socket client = this->acc_socket.accept_client();
    while (this->on) {
      stage.update();
      StageDTO s = stage.get_stageDTO();

      YAML::Emitter out;
      out << YAML::BeginMap;
      out << YAML::Key << "stage";
      out << YAML::Value << s;
      out << YAML::EndMap;

      client.send_dto(out.c_str());*/


  YAML::Node node;
  node["worm_turn"] = s.worm_turn;
  YAML::Node worms;
  for (auto &w : s.worms) {
    worms[w.first] = w.second;
  }
  node["worms"] = worms;
  for (auto &b : s.beams) {
    node["beams"].push_back(b);
  }
  for (auto &b : s.beams) {
    node["weapons"].push_back(b);
  }
  std::string str;
  node >> str;
  return str;
}
