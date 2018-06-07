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
#include "stage.h"



class Server {
private:
  std::thread sending_thread;
  std::thread receiving_thread;
  Socket client;
  Stage stage;

  bool on;
public:
  Server(Socket socket);
  virtual ~Server();
  void start();
  void send();
  void receive();
  void stop();
};

#endif /* SERVER_H_ */