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
  Socket socket(NULL,port);
  socket.bind_and_listen();
  Socket client = socket.accept_socket(); //esto despues va estar en el game una vez por cada jugador que se conecte
  std::string s("file.yaml");
  Game game(s, client);
  Socket client2 = socket.accept_socket();
  game.add_player(client2);
  //game.start();
  //este thread espera que se ingrese "q" para terminar ejecución
  while (getchar() != 'q') {
    continue;
  }
  game.stop();
  socket.shut();
  return 0;
}
