#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <iostream>
#include <vector>
#include "Logger.h"

#define PROTOCOL_MSG_SIZE 4
#define CHUNK_LEN 512

class Socket {
  private:
    int socket_num;
    const char * host_name;
    const char * port;
    bool is_connected;
    
  //data una estructura del tipo addrinfo, la recorre hasta 
  //poder crear un socket y setea entonces el numero del socket
  struct addrinfo * define_socket_num(struct addrinfo * node);

  //crea un socket inicial
  void init();

  //devuelve el primer nodo de addrinfo
  struct addrinfo * addrinfo();

  //valida que el puerto sea valido
  bool is_valid_port(const char * port);



public:
  //Constructor por copia anulado
  Socket(const Socket& other) = delete;
  //Asignacion por copia anulada
  Socket& operator=(const Socket &other) = delete;
  //Constructor por movimiento anulado
  Socket(Socket&& other); 
  //Asignacion por movimiento anulada
  Socket& operator=(Socket&& other);

  //constructor del socket
  Socket(const char * host_name, const char * port);

  //constructor del peer socket ya conectado
  explicit Socket(int socket_num);

  //trata de conectar al socket
  void connection();

  //sirve para servidores, 
  //el socket se quedara esperando que se conecten con el 
  void bind_and_listen();

  //sirve para servidores,
  //acepta a otro socket que quiera conectarse con el
  Socket accept_socket();

private:
  int get_digits(unsigned int num);
  ssize_t receive_size_first();
  void send_size_first(unsigned int size);

  //Debe estar conectado con el socket cuyo numero sea skt_num
  //porque a partir de él recibirá datos
  //Tratará de recibir tantos  datos como se especifique
  //en el parametro size.
  //guarda los datos recibidos en buffer que debe ser un array
  //de un tamaño mayor  o igual a size
  int receive_buffer(char* buffer, const size_t size);


  //Tratará de enviar tantos datos como se especifique
  //en el parámetro size.
  //enciará los datos en buffer que debe ser un array
  //de un tamaño mayor o igual a size
  int send_buffer(const char* buffer,const size_t size);


  
  int get_hosts(struct addrinfo **result, const char* port, const char* host);

public:
  void send_dto(const std::string & dto_to_send);
  
  std::string receive_dto();

  void stop();

  //destructor del socket
  ~Socket();
};

#endif