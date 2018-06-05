/*
 * Game.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#include "Game.h"

#include "BlockingQueue.h"
#include "Constants.h"
#include "DTOs.h"

Game::Game(std::string &stage_name, Socket &client) :
           stage(stage_name), players(Player(client)),
           stage_queue(Constants::max_blocking_queue) {
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

void Game::prepare() {
  //crear colas bloqueantes
  //colas de los jugadores
  for (auto& p : this->players) {
    BlockingQueue<StageDTO>* q = new BlockingQueue<StageDTO>(Constants::max_blocking_queue);
    this->players_queues.push_back(q);
    p.add_stage_queues(q, &(this->stage_queue));
    p.start();
  }
  this->timer.run();
}

void Game::run() {
  // inicializar players, colas, timer
  prepare();

  while (!this->stage.finished()) {
    // sacar action de la cola: action de player o action del timer(update)
    ActionDTO &action = this->stage_queue.pop();
    if (action.type == Timer_update)
      this->stage.update();
    else
      this->stage.make_action(action);
    for (auto &q : this->players_queues) {
      StageDTO s = stage.get_stageDTO();
      q->push(s);
    }
  }

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
