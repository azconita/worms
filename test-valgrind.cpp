/*
 * test-valgrind.cpp
 *
 *  Created on: Jun 24, 2018
 *      Author: gabi
 */

//#include "Box2D/Box2D.h"
#include "Logger.h"

logger oLog("server.log");

int main(int argc, char *argv[]) {
  oLog() <<"asdf";
  while (getchar() != 'q') {
    continue;
  }
  return 0;
}


