#include "Server.h"
#include "Logger.h"

logger oLog("server.log");

int main(int argc, char *argv[]) {
  //Inicializa servidor con puerto
  Server server(argv[1]);
  //ejecuta servidor en nuevo thread
  server.start();
  //este thread espera que se ingrese "q" para terminar ejecución
  while (getchar() != 'q') {
    continue;
  }
  server.stop();
  return 0;
}
