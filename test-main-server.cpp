/*
 * test-main-server.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "common_socket.h"
#include "Game.h"

int main(int argc, char* argv[]) {
  Socket mock_client();
  Game game("file.yaml", &mock_client);
  game.start();

}

