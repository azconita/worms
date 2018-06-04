/*
 * Server.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#include "Server.h"

#include "Constants.h"
#include "Stage.h"

Server::Server(const std::string port) : acc_socket(port) {
  // inicializar vector de escenarios posibles
  for (auto &s : Constants::stages) {
    this->games_by_stage[s] = std::vector();
  }
}

Server::~Server() {
  // TODO Auto-generated destructor stub
  this->acc_socket.shut();
  this->thread.join();
}

void Server::start() {
  this->thread = std::thread(&Server::run, this);
}

void Server::run() {
  while (this->on) {
    Socket client = this->acc_socket.accept_client();
    std::string stage = client.recv_string();
    std::map<std::string,std::vector<Game*>>::iterator it = this->games_by_stage.find(stage);
    if (it == this->games_by_stage.end()) {
      // no se encontro el stage: responder al cliente
    } else {
      if (it->second.empty()) {
        // no existen partidas de este escenario: crear una
        it->second.push_back(new Game(stage, client));
      } else {
        // buscar si hay partida con lugar
        bool found = false;
        for (auto &game : it->second) {
          if (game->not_full()) {
            // partida no llena: agregar jugador
            game->add_player(client);
            if (!game->not_full())
              game->start();
            found = true;
            break;
          }
        }
        if (! found) {
          // crear nueva partida
          it->second.push_back(new Game(stage, client));
        }
      }
    }


  }
}

void Server::stop() {
  this->on = false;
}

