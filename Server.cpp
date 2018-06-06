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
  this->on = true;

}

void Server::start() {
  this->sending_thread = std::thread(&Server::send, this);
  this->receiving_thread = std::thread(&Server::receive, this);
}


void Server::send() {
  Stage stage("file.yaml");
  Socket client = this->acc_socket.accept_client();
  extern logger oLog;
  
  while (this->on) {
    stage.update();
    StageDTO s = stage.get_stageDTO();
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "stage";
    out << YAML::Value << s;
    out << YAML::EndMap;
    try{
      client.send_dto(out.c_str());
    }catch(Error e){
        oLog() << "Player quit (peer socket closed).";
        stop();
        break;
    }
  }
}

void Server::receive(){
  extern logger oLog;
  while(this->on){
    oLog() << "reciviendo en otro hilo";
  }

}


void Server::stop() {
  this->on = false;
}

Server::~Server() {
  this->acc_socket.shut();
  this->sending_thread.join();
  this->receiving_thread.join();
}
