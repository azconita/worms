/*
 * Server.h
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <string>
#include "server_socket.h"
#include "DTOs.h"

class Server {
private:
  std::thread thread;
  ServerSocket acc_socket;
  bool on = true;
public:
  Server(const std::string port);
  virtual ~Server();
  void start();
  void run();
  void stop();
};

#endif /* SERVER_H_ */