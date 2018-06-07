#include "ServerSocket.h"
#include <string>
#include <arpa/inet.h>
#include "Socket.h"

ServerSocket::ServerSocket(const std::string port) : Socket(NULL, port) {
  (this->bind_and_listen() == 0);
}

ServerSocket::ServerSocket(ServerSocket&& other) :
        Socket(std::move(other)) {
}

ServerSocket::ServerSocket(Socket&& other) :
        Socket(std::move(other)) {
}

ServerSocket::~ServerSocket() {
  this->shut();
}

Socket ServerSocket::accept_client() {
  Socket s = this->accept_connection();
  return s;
}
