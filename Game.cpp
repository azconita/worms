/*
 * Game.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#include "Game.h"

#include "BlockingQueue.h"
#include "Constants.h"
#include "Dtos.h"

Game::Game(std::string &stage_name, Socket &client) :
           stage(stage_name),
           stage_queue(100), timer(stage_queue) {
  this->players.push_back(new Player(client));
}

Game::~Game() {
  // TODO Auto-generated destructor stub
  this->timer.join();
  //delete players!
  for (auto &q : this->players_queues) {
    delete q;
  }
  for (auto &p : this->players) {
    delete p;
  }
}

bool Game::not_full() {
  return (this->players.size() < this->limit);
}

void Game::add_player(Socket &client) {
  if (this->players.size() >= this->limit)
    return;

  printf("[Game] add_player -> new Player\n");
  this->players.push_back(new Player(client));
  //TODO: init game? add worms to initiated game?
  //if (this->players.size() == this->limit)
   // this->start();
}

void Game::prepare() {
  //set players in stage
  this->stage.set_worms_to_players(this->players.size());
  //crear colas bloqueantes de los jugadores
  int i = 0;
  for (auto& p : this->players) {
    p->set_id(i);
    i++;
    printf("[Game] prepare -> new BlockingQueue\n");
    BlockingQueue<StageDTO>* q = new BlockingQueue<StageDTO>(100);
    this->players_queues.push_back(q);
    p->add_stage_queues(q, &(this->stage_queue));
    printf("[Game]starting player\n");
    p->start();
  }
  this->timer.start();

}

void Game::run() {
  // inicializar players, colas, timer
  prepare();
  // start game!
  while (!this->stage.finished()) {
    // sacar action de la cola: action de player o action del timer(update)
    ActionDTO action = this->stage_queue.pop();
    //printf("[Game] pop action: %d\n", action.type);
    StageDTO s;
    if (action.type == Quit) {
      //end game: send block with endgame??
      this->stage.end();
      s.worm_turn = -1;
      this->timer.stop();
    } else {
      if (action.type == Timer_update)
        this->stage.update();
      else
        this->stage.make_action(action);
      s = stage.get_stageDTO();
    }
    for (auto &q : this->players_queues) {
      q->push(s);
    }
  }
  int win = this->stage.get_winner();
  if (win != -1) {
    for (auto& p : this->players)
      p->end_game(win);
  }
}

void Game::stop() {
  this->stage.end();
}
