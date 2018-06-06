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
#include "Error.h"
#include "ServerSocket.h"
#include "Dtos.h"

class Server {
private:
  std::thread sending_thread;
  std::thread receiving_thread;
  ServerSocket acc_socket;
  bool on;
public:
  Server(const std::string port);
  virtual ~Server();
  void start();
  void send();
  void receive();
  void stop();
};

#endif /* SERVER_H_ */