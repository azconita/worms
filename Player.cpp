/*
 * Player.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#include "Player.h"

#include "Socket.h"
#include "Dtos.h"
#include "Error.h"
#include <yaml-cpp/yaml.h>
//#include "PlayerReceiver.h"
//#include "PlayerSender.h"

Player::Player(Socket &client) : client(client) {
  // TODO Auto-generated constructor stub

}

Player::~Player() {
  // TODO Auto-generated destructor stub
  this->sender.join();
  this->receiver.join();
}

void Player::start() {
  this->sender = std::thread(&Player::send, this);
  this->receiver = std::thread(&Player::receive, this);
}
void Player::send() {

  extern logger oLog;
  while (true) {
    StageDTO s = this->send_queue->pop();
    printf("[Player] send stage\n");
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "stage";
    out << YAML::Value << s;
    out << YAML::EndMap;
    try{
      //printf("se envia %s\n", out.c_str());
      this->client.send_dto(out.c_str());
    }catch(Error e){
        oLog() << "Player quit (peer socket closed).";
        //this->stop();
    }
  }
}

void Player::receive(){
  extern logger oLog;
  while (true) {
    try{
      std::string action_str = this->client.receive_dto();
      printf("%s\n",action_str.c_str() );
      oLog() << "recibiendo";
      YAML::Node yaml_received = YAML::Load(action_str);
      ActionDTO action_received = yaml_received["action"].as<ActionDTO>();
      this->recv_queue->push(action_received);
      if(action_received.type == Make_move){
        printf("saltaar\n" );
      }
    }catch(Error e){
        //stop();
    }
  }
}

void Player::add_stage_queues(BlockingQueue<StageDTO> *send_queue,
                        BlockingQueue<ActionDTO> *recv_queue) {
  this->send_queue = send_queue;
  this->recv_queue = recv_queue;
}
