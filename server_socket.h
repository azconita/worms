#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H value

#include "common_socket.h"
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

  std::string recv_filename();
  std::string recv_tag();
  std::string recv_hash();
  int recv_tagquantity();
  int recv_respcode();

  Socket accept_client();
};

#endif
