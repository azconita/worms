/*
 * server.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#include "Server.h"
#include "Logger.h"

logger oLog("server.log");

int main(int argc, char *argv[]) {
  

  char * port = argv[1];
  Socket socket(NULL,port);
  socket.bind_and_listen();
  Socket client = socket.accept_socket(); //esto despues va estar en el game una vez por cada jugador que se conecte

  //Inicializa servidor con un cliente
  Server server(std::move(client));
  //ejecuta servidor en nuevo thread
  server.start();
  //este thread espera que se ingrese "q" para terminar ejecución
  while (getchar() != 'q') {
    continue;
  }
  server.stop();
  socket.shut();
  return 0;
}