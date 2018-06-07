/*
 * Server.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */
#include "Server.h"

#include "Constants.h"
#include "Stage.h"
#include "Dtos.h"
#include "Socket.h"

Server::Server(const std::string port) :
                     acc_socket(NULL, port.c_str()) {
  // inicializar vector de escenarios posibles
  for (auto &s : Constants::stages) {
    this->games_by_stage[s] = std::vector<Game*>();
  }
}

void Server::start() {
}


void Server::run() {
  while (this->on) {
    Socket client = this->acc_socket.accept_socket();
    std::string stage = client.receive_dto();
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
