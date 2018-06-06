/*
 * Server.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#include "Server.h"
#include "stage.h"
#include "Dtos.h"

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
  extern logger oLog;
  Stage stage("file.yaml");
  Socket client = this->acc_socket.accept_client();
  while (this->on) {
    stage.update();
    StageDTO s = stage.get_stageDTO();
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "stage";
    out << YAML::Value << s;
    out << YAML::EndMap;

    try{
      oLog() << "senfing StageDTO: " << out.c_str();
      client.send_dto(out.c_str());
    }catch(Error e){
        oLog() << "Player quit (peer socket closed).";
    }
  }
}

void Server::stop() {
  this->on = false;
}

