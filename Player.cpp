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

Player::Player(Socket client) : client(std::move(client)) {
  this->on = false;
}

Player::~Player() {
  oLog() << "[Player] deleted: " << this->id << "\n";
  this->sender.join();
  this->receiver.join();
  //this->client.shut();
}

void Player::set_id(int id) {
  this->id = id;
}

void Player::start() {
  this->on = true;
  this->sender = std::thread(&Player::send, this);
  this->receiver = std::thread(&Player::receive, this);
}

void Player::send() {
  extern logger oLog;
  while (this->on) {
    StageDTO s = this->send_queue->pop();
    s.player_id = this->id;
    if (s.worm_turn == -1) {
      this->stop();
    }
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "stage";
    out << YAML::Value << s;
    out << YAML::EndMap;
    try {
      this->client.send_dto(out.c_str());
      if (s.winner != -1) {
        oLog() << "[Player] the winner: "<< s.winner <<"\n";
        this->stop();
        this->client.shut();
      }
    } catch(Error e) {
      oLog() << "Player quit (peer socket closed).";
      this->stop();
    }
  }
  oLog() << "[Player] se termino el ciclo send: " << this->id << "\n";
}

void Player::receive(){
  extern logger oLog;
  while (this->on) {
    try {
      std::string action_str = this->client.receive_dto();
      oLog() << "recibiendo";
      oLog() << action_str.c_str();
      YAML::Node yaml_received = YAML::Load(action_str);
      ActionDTO action_received = yaml_received["action"].as<ActionDTO>();
      action_received.player_id = this->id;
      this->recv_queue->push(action_received);
    } catch(Error e) {
      ActionDTO a;
      a.type = Quit;
      a.player_id = this->id;
      oLog() << "[Player] receive: pushing quit action\n";
      this->recv_queue->push(a);
      stop();
    }
  }
  oLog() << "[Player] se termino el ciclo receive: " << this->id << "\n";
}

void Player::add_stage_queues(BlockingQueue<StageDTO> *send_queue,
                        BlockingQueue<ActionDTO> *recv_queue) {
  this->send_queue = send_queue;
  this->recv_queue = recv_queue;
}

void Player::stop(){
  this->on = false;
}

