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
#include "Dtos.h"
#include "Game.h"
#include "Socket.h"

class Server {
private:
  std::thread thread;
  Socket acc_socket;
  bool on = true;

  std::map<std::string,std::vector<Game*>> games_by_stage;
  // deberia existir para no perder referencias?
  //std::vector<Socket> clients;
public:
  Server(const std::string port);
  virtual ~Server();
  void start();
  void run();
  void stop();
private:
  std::string get_yaml(StageDTO &s);
};

#endif /* SERVER_H_ */
