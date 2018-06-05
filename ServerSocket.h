#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H value

#include "Socket.h"
#include <string>
#include <vector>

class ServerSocket : public Socket {
public:
  explicit ServerSocket(std::string port);
  ServerSocket(ServerSocket&& other);
  explicit ServerSocket(Socket&& other);
  ~ServerSocket();
  ServerSocket(const ServerSocket&) = delete;
  ServerSocket& operator=(const ServerSocket&) = delete;

  Socket accept_client();
};

#endif