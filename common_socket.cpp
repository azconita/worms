#include "common_socket.h"
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

int Socket::send_buffer(size_t size, unsigned char *buffer) {
  int sent, total_sent = 0;

  while ((sent = send(this->sock, &buffer[total_sent],
            size - total_sent, MSG_NOSIGNAL)) > 0) {
    total_sent += sent;
  }
  // if sent == 0: socket closed
  if (sent < 0)
    return -1;
  return total_sent;
}

int Socket::receive_buffer(size_t size, unsigned char *buffer) {
  int received(0), total_received = 0;
  while ((size - total_received > 0) &&
        ((received = recv(this->sock, (void*) &buffer[total_received],
              size - total_received, MSG_NOSIGNAL)) > 0)) {
    total_received += received;
  }
  // if received == 0: socket closed
  if (received < 0) {
    return -1;
  }
  return total_received;
}

int Socket::recv_int() {
  int r;
  if (this->receive_buffer(sizeof(int), (unsigned char *) &r)
                        < (int) sizeof(int))
    return -1;
  return ntohl(r);
}

std::string Socket::recv_string() {
  int size = this->recv_int();
  unsigned char * buffer = new unsigned char[size+1]();
  this->receive_buffer(size, buffer);
  buffer[size] = '\0';
  std::string s(reinterpret_cast<char*>(buffer));
  delete[](buffer);
  return s;
}

void Socket::send_int(int i) {
  int s = htonl(i);
  this->send_buffer(sizeof(int), (unsigned char *) &s);
}

void Socket::send_string(std::string &s) {
  this->send_int(s.size());
  this->send_buffer(s.size(), (unsigned char *) s.c_str());
}


void Socket::send_file(std::string &filename) {
  std::ifstream ifile(filename, std::ifstream::binary);
  ifile.seekg(0, ifile.end);
  size_t size = ifile.tellg();
  ifile.seekg(0, ifile.beg);
  unsigned char * buffer = new unsigned char[size]();
  ifile.read((char *) buffer, size);
  this->send_int(size);
  this->send_buffer(size, buffer);
  ifile.close();
  delete[] buffer;
}

void Socket::recv_file(std::string &filename) {
  std::ofstream ofile(filename, std::ifstream::binary);
  int size = this->recv_int();
  unsigned char * buffer = new unsigned char[size]();
  this->receive_buffer(size, buffer);
  ofile.write((char*) buffer, size);
  delete[] buffer;
  ofile.close();
}
