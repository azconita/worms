#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <iostream>
#include <vector>

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

  //Wrappers para enviar/recibir int, string y file
  int Socket::receive_message(char* buffer, const size_t size);
  int Socket::send_message(const char* buffer,const size_t size);

private:
  int get_hosts(struct addrinfo **result, const char* port, const char* host);
};

#endif