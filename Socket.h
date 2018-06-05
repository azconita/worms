#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <iostream>
#include <vector>
#include "Logger.h"

#define PROTOCOL_MSG_SIZE 4
#define CHUNK_LEN 512

class Socket {
protected:
  int sock;

public:
  Socket();
  explicit Socket(int sock);
  Socket(Socket &&other);
  ~Socket();


  //No se permiten copias, solo por movimiento
  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  int bind_and_listen(const char* port);
  int connect_to_server(const char* host, const char* port);
  Socket accept_connection();
  bool not_valid();
  void shut();
  int send_buffer(size_t size, unsigned char *buffer);
  int receive_buffer(size_t size, unsigned char *buffer);

  void send_dto(const std::string & dto_to_send);
  std::string receive_dto();


private:
  int get_digits(unsigned int num);
  ssize_t receive_size_first();
  void send_size_first(unsigned int size);
  int receive_buffer(char* buffer, const size_t size);
  int send_buffer(const char* buffer,const size_t size);
  
  int get_hosts(struct addrinfo **result, const char* port, const char* host);
};

#endif