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

Server::Server(char* port) :
                     acc_socket(NULL, port) {
  // inicializar vector de escenarios posibles
  oLog() << "[Server] Inicializing Server\n";
  acc_socket.bind_and_listen();
  for (auto &s : Constants::stages) {
    this->games_by_stage[s.first] = std::vector<Game*>();
  }
}

Server::~Server() {
  oLog() << "[Server] destroy\n";
  for (auto &i : this->games_by_stage) {
    for (auto &g : i.second) {
      delete g;
      oLog() << "[Server] game joined\n";
    }
    i.second.clear();
  }
}


void Server::run() {
  oLog() << "running Server\n";
  while (this->on) {
    try {
      Socket client = this->acc_socket.accept_socket();
      std::string stage = client.receive_dto();
      oLog() << "[Server] new client, stage: " << stage.c_str() << "\n";
      std::map<std::string,std::vector<Game*>>::iterator it = this->games_by_stage.find(stage);
      if (it == this->games_by_stage.end()) {
        // no se encontro el stage: responder al cliente
      } else {
        if (it->second.empty()) {
          oLog() <<"[Server] create stage\n";
          // no existen partidas de este escenario: crear una
          it->second.push_back(new Game(stage, Constants::stages.at(stage), std::move(client)));
        } else {
          // buscar si hay partida con lugar
          bool found = false;
          for (auto &game : it->second) {
            if (game->not_full() && !found) {
              // partida no llena: agregar jugador
              game->add_player(std::move(client));
              if (game->ready()) {
                oLog() << "[Server] starting game\n";
                game->start();
              }
              found = true;
              //break;
            }
          }
          if (! found) {
            // crear nueva partida
            it->second.push_back(new Game(stage, Constants::stages.at(stage), std::move(client)));
          }
        }
      }
    } catch (Error &e) {
      oLog() << "[Server] Error: " << e.what();
    }
  }
}


void Server::stop() {
  this->on = false;
  this->acc_socket.shut();
}
