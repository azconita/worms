/*
 * Server.h
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <map>
#include <string>
#include <thread>
#include <vector>

#include "CommonThread.h"
#include "Dtos.h"
#include "Game.h"
#include "Socket.h"
#include "Logger.h"

extern  logger oLog;

class Server : public Thread {
private:
  Socket acc_socket;
  bool on = true;

  std::map<std::string,std::vector<Game*>> games_by_stage;
  // deberia existir para no perder referencias?
  //std::vector<Socket> clients;
public:
  Server(char* port);
  virtual ~Server();
  //void start();
  void run();
  void stop();
};

#endif /* SERVER_H_ */
