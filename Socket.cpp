#include "Socket.h"
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <string>

Socket::Socket() {
  this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sock == -1)
      throw -1;
}

Socket::Socket(Socket &&other) {
  this->sock = other.sock;
  other.sock = -1;
}

Socket::Socket(int sock) : sock(std::move(sock)) {}

int Socket::get_hosts(struct addrinfo **result, const char* port,
                      const char* host) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;       // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP
  if (getaddrinfo(host, port, &hints, result) != 0)
    throw -1;
  return 0;
}


int Socket::bind_and_listen(const char* port) {
  struct addrinfo *results, *res;
  int s;
  bool not_bound = true;
  if (this->get_hosts(&results, port, NULL) != 0)
    return 1;
  for (res = results; res != NULL && not_bound; res = res->ai_next) {
    s = bind(this->sock, res->ai_addr, res->ai_addrlen);
    if (s == 0)  {
      not_bound = false;
      break;
    }
  }
  freeaddrinfo(results);
  if (s != 0) {
    close(this->sock);
    return -1;
  }

  if (listen(this->sock, 1) == -1) { // cuántos?
    close(this->sock);
    return -1;
  }
  return 0;
}


int Socket::connect_to_server(const char* host, const char* port) {
  struct addrinfo *results, *res;
  int s;
  if (this->get_hosts(&results, port, host) != 0)
    return 1;
  for (res = results; res != NULL; res = res->ai_next) {
    if ((s = connect(this->sock, res->ai_addr, res->ai_addrlen)) != -1)
      break;
    //else
      //std::cout << "[Error] " << strerror(errno) << '\n';
  }
  freeaddrinfo(results);
  if (s == -1) {
    close(this->sock);
    //throw -1;
    return -1;
  }
  return 0;
}

Socket Socket::accept_connection() {
  struct sockaddr_un peer_addr;
  socklen_t peer_addr_size;
  int s;
  peer_addr_size = sizeof(struct sockaddr_un);
  if ((s = accept(this->sock, (struct sockaddr *) &peer_addr,
          &peer_addr_size)) != -1) {
    return Socket(s);
  } else {
    //std::cout << "[error] [Socket] accept_connection: " << strerror(errno)
    //        << '\n';
    return Socket(-1);
    //throw -1;
  }
}

bool Socket::not_valid() {
  return (this->sock == -1);
}

Socket::~Socket() {
  if (this->sock != -1)
    close(this->sock);
}

void Socket::shut() {
  shutdown(this->sock, SHUT_RDWR);
}



int Socket::receive_message(char* buffer, const size_t size){
    int total_received = 0;
    int bytes_recived = 0;


    while ((bytes_recived = recv(this->sock, //
    &buffer[total_received],//
    size - total_received, MSG_NOSIGNAL)) >0) {
      total_received += bytes_recived;
      if (size -total_received== 0){
        break;
      }
    }
    if (bytes_recived < 0) { 
      throw Error(" Error recibing info: %s\n", strerror(errno));
    }
    return total_received;
}


int Socket::send_message(const char* buffer,const size_t size){
    int total_sent = 0;
    int bytes_sent = 0;

    while ((bytes_sent = send(this->sock, //
      &buffer[total_sent], size-total_sent, MSG_NOSIGNAL)) >0){
        total_sent += bytes_sent;
    }
    if (bytes_sent < 0) { 
      throw Error(" Error sending info: %s\n", strerror(errno));
    }
    return total_sent;
}
