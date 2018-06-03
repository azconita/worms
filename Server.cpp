/*
 * Server.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#include "Server.h"
#include "stage.h"

Server::Server(const std::string port) : acc_socket(port) {
  // TODO Auto-generated constructor stub

}

Server::~Server() {
  // TODO Auto-generated destructor stub
  this->acc_socket.shut();
  this->thread.join();
}

void Server::start() {
  this->thread = std::thread(&Server::run, this);
}

void Server::run() {
  Stage stage("file.yaml");
  Socket client = this->acc_socket.accept_client();
  while (this->on) {
    stage.update();
    StageDTO s = stage.get_stageDTO();
    std::string str = this->get_yaml(s);
    client.send_string(str);
  }
}

void Server::stop() {
  this->on = false;
}

//private
std::string Server::get_yaml(StageDTO &s) {
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
