/*
 * server.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#include "Game.h"
#include "Logger.h"
#include "Socket.h"

logger oLog("server.log");

int main(int argc, char *argv[]) {

  char * port = argv[1];
  std::string filename("file.yaml");
  Socket socket(NULL,port);
  printf("holiss\n");
  socket.bind_and_listen();
  Socket client = socket.accept_socket(); //esto despues va estar en el game una vez por cada jugador que se conecte
  Game game(filename, client);
  game.start();
   //este thread espera que se ingrese "q" para terminar ejecución
  while (getchar() != 'q') {
    continue;
  }

  socket.stop();
  return 0;
}
