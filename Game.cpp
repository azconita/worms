/*
 * Game.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#include "Game.h"

Game::Game(std::string &stage_name, Socket &client) :
            stage(stage_name), players(Player(client)) {
  // TODO Auto-generated constructor stub

}

Game::~Game() {
  // TODO Auto-generated destructor stub
}

bool Game::not_full() {
  return (this->players.size() < this->limit);
}

void Game::add_player(Socket client) {
  if (this->players.size() >= this->limit)
    return;

  this->players.push_back(Player(client));
  //TODO: init game? add worms to initiated game?
  if (this->players.size() == this->limit)
    this->start();
}

void Game::start() {
  //Stage stage("file.yaml");
  this->stage.update();
  //asignar gusanos a los jugadores
  //threads de sockets?



  //StageDTO s = stage.get_stageDTO();
  //std::string str = this->get_yaml(s);
  //this->players.send_string(str);
}


std::string Game::get_yaml(StageDTO &s) {
  YAML::Node node;
  node["worm_turn"] = s.worm_turn;
  YAML::Node worms;
  for (auto &w : s.worms) {
    worms[w.first] = w.second;
  }
  node["worms"] = worms;
  for (auto &b : s.beams) {
    node["beams"].push_back(b);
  }
  for (auto &b : s.beams) {
    node["weapons"].push_back(b);
  }
  std::string str;
  node >> str;
  return str;
}
