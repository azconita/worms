/*
 * Player.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#include "Player.h"

#include "common_socket.h"
#include "DTOs.h"
#include "PlayerReceiver.h"
#include "PlayerSender.h"

Player::Player(Socket &client) : client(client) {
  // TODO Auto-generated constructor stub

}

Player::~Player() {
  // TODO Auto-generated destructor stub
  this->sender.join();
  this->receiver.join();
}

void Player::start() {
  this->sender = std::thread{PlayerSender(this->client, this->send_queue, this->recv_queue)};
  this->receiver = std::thread{PlayerReceiver(this->client, this->send_queue, this->recv_queue)};

}
