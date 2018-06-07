/*
 * Server.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#include "Server.h"
#include "stage.h"
#include "Dtos.h"

Server::Server(Socket peer) : 
  stage("file.yaml"),
  client(std::move(peer)){
  this->on = true;
}

void Server::start() { 
  this->sending_thread = std::thread(&Server::send, this);
  this->receiving_thread = std::thread(&Server::receive, this);
}


void Server::send() {

  extern logger oLog;
  
  while (this->on) {
    this->stage.update();
    StageDTO s = stage.get_stageDTO();
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "stage";
    out << YAML::Value << s;
    out << YAML::EndMap;
    try{
      printf("se envia %s\n", out.c_str());
      this->client.send_dto(out.c_str());
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
    oLog() << "recibiendo";
  }

}


void Server::stop() {
  this->on = false;
}

Server::~Server() {
  this->sending_thread.join();
  this->receiving_thread.join();
}
