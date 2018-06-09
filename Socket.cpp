#include "Socket.h"
#include "Error.h"
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


#define SUCCESS 0
#define ERROR 1


struct addrinfo * Socket::define_socket_num(struct addrinfo * node){
    while (node != NULL) {
      socket_num = //
        socket(node->ai_family, //
        node->ai_socktype, //
        node->ai_protocol);
      if (socket_num != -1) {
          this->socket_num = socket_num;
         return node;
      } 
      node = node->ai_next;
    }
    return NULL;  
}



void Socket::init(){
    struct addrinfo *addrinfoNode = addrinfo();
    if(define_socket_num(addrinfoNode)==NULL){
      throw Error("Error en init\n");
    }
    free(addrinfoNode); 
}

struct addrinfo * Socket::addrinfo(){
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo)); 
    hints.ai_family = AF_INET;       //IPv4 
    hints.ai_socktype = SOCK_STREAM; //TCP 
    hints.ai_flags = 0; 

    struct addrinfo *addrinfoNode;
    

    int addrinfo_returnvalue = //
    getaddrinfo(this->host_name, this->port, &hints, &addrinfoNode);

    if (addrinfo_returnvalue != SUCCESS){
      gai_strerror(addrinfo_returnvalue);
    }
    return addrinfoNode;
} 

bool Socket::is_valid_port(const char * port){
  for (unsigned int i = 0; i < strlen(port); i ++){
    if (!isdigit(port[i])){
      return false;
    }
  }
  return true;
}




Socket::Socket(const char * host_name, const char * port){
    if(!is_valid_port(port)){
      throw Error("%s no es un purto valido\n"//
       "deben ser todos caracteres numéricos", port);
    }

    this->host_name = host_name;
    this->port = port;
    this->is_connected = false;

    init();
    int val = 1;
    if (setsockopt(this->socket_num, SOL_SOCKET, SO_REUSEADDR, &val, //
        sizeof(val)) == -1) {
        close(this->socket_num);
        throw Error("Error in reuse socket: %s\n", strerror(errno));
    }
}

Socket::Socket(int socket_num){
  this->is_connected = true;
  this->socket_num = socket_num;
}



Socket::Socket(Socket&& other){
  this->socket_num = other.socket_num;
  this->host_name = other.host_name;
  this->port = other.port;
  this->is_connected = other.is_connected;

  other.socket_num =-1;
  other.host_name ="null";
  other.port ="null";
  other.is_connected = false;
}
  
Socket& Socket::operator=(Socket&& other){
  if (this == &other) {
    return *this; // other is myself!
  }
  this->socket_num = other.socket_num;
  this->host_name = other.host_name;
  this->port = other.port;
  this->is_connected = other.is_connected;

  other.socket_num =-1;
  other.host_name ="null";
  other.port ="null";
  other.is_connected = false;
  return *this;
}


void Socket::connect_to_server(){
  struct addrinfo *addrinfoNode = addrinfo();

  struct addrinfo *node  = define_socket_num(addrinfoNode);
  
  while (node != NULL && this->is_connected == false) {
      int connection_returnvalue = connect(this->socket_num, //
        node->ai_addr, node->ai_addrlen);
      this->is_connected = (connection_returnvalue != -1);
      if (this->is_connected){
          break;
      }
      node = define_socket_num(addrinfoNode);
  }
  freeaddrinfo(addrinfoNode);
  if (this->is_connected == false) {
      close(this->socket_num);
      throw Error("Error in socket connection: %s\n", strerror(errno));
  }
}

void Socket::bind_and_listen(){
    struct addrinfo *addrinfoNode = addrinfo();

    int bindReturnValue = -1;
    while (addrinfoNode != NULL) {
      bindReturnValue = bind(this->socket_num, //
      addrinfoNode->ai_addr, //
      addrinfoNode->ai_addrlen);
      if (bindReturnValue != -1) {
          break;
      }
      addrinfoNode = addrinfoNode->ai_next;
    }
    free(addrinfoNode);

    if (bindReturnValue == -1) {
      throw Error("Error in bind: %s\n", strerror(errno));
    }
    int listenReturnValue = listen(this->socket_num, 20);
    if (listenReturnValue == -1) {
      throw Error("Error in listen: %s\n", strerror(errno));
    }
    this-> is_connected = true;
}

Socket Socket::accept_socket(){
    int new_sockfd = accept(this->socket_num, NULL, NULL);
    if (new_sockfd == -1){
      throw Error("Error in accept");
    }
    return std::move(Socket(new_sockfd));
}


void Socket::shut(){
  shutdown(this->socket_num, SHUT_RDWR);
  close(this->socket_num);
}

Socket::~Socket(){
  if (this->socket_num > 0){
    shutdown(this->socket_num, SHUT_RDWR);
    close(this->socket_num);
  } 
}
////-///////////comunicacion interna///////////////////////

int Socket::receive_size_first(){
  int msg_size;
  if(this->receive_buffer((char *) &msg_size, sizeof(int)) < (int) sizeof(int)){
    throw Error("Socket: error al recibir el tamaño  %i del dto\n",ntohl(msg_size));
  }
  return ntohl(msg_size);
}


void Socket::send_size_first(unsigned int size){
  int msg_size = htonl(size);
  this->send_buffer((char *) &msg_size, sizeof(int));
}



int Socket::receive_buffer(char* buffer, const size_t size){
    int total_received = 0;
    int bytes_recived = 0;

    while ((bytes_recived = recv(this->socket_num, //
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


int Socket::send_buffer(const char* buffer,const size_t size){
    int total_sent = 0;
    int bytes_sent = 0;

    while ((bytes_sent = send(this->socket_num, //
      &buffer[total_sent], size-total_sent, MSG_NOSIGNAL)) >0){
        total_sent += bytes_sent;
    }
    if (bytes_sent < 0) { 
      throw Error(" Error sending info: %s\n", strerror(errno));
    }
    return total_sent;
}

//////////////////protocolo de comunicacion/////////////////////////////////

void Socket::send_dto(const std::string & dto_to_send){
    int dto_size = dto_to_send.size();
    send_size_first(dto_size);
   
    int bytes_sent = 0;
    int total_sent = 0;
    char request[CHUNK_LEN+1];

    while (total_sent < dto_size ){

      int request_len = CHUNK_LEN;
      if (dto_size - total_sent < CHUNK_LEN){
        request_len = dto_size - total_sent;
      } 

      std::string dto_chunk = dto_to_send.substr(total_sent,bytes_sent +request_len);
      memcpy(request, dto_chunk.c_str() , request_len);
      bytes_sent = send_buffer(request, request_len);
        
      if (bytes_sent < 0){
        break;
      }
      total_sent += bytes_sent;
    }
}

std::string Socket::receive_dto(){
  //  printf("numero del socket ->-----> %i\n",this->socket_num );
   std::string dto_received;
  int dto_size = receive_size_first();
  char chunk[CHUNK_LEN+1];
  int total_received = 0;
  int bytes_received = 0;
  while (total_received < dto_size){
        memset(chunk, 0, CHUNK_LEN+1);

        int request_len = CHUNK_LEN;
        if (dto_size - total_received < CHUNK_LEN){
            request_len = dto_size - total_received;
        }
        bytes_received = receive_buffer(chunk, request_len); 
        
        total_received +=bytes_received;
        if (bytes_received <= 0){
          break;
        }
        dto_received.append(chunk);

    }
  return dto_received; 
}


