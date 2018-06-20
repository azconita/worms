/*
 * Game.cpp
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#include "Game.h"


Game::Game(std::string &stage_name, int total_players, Socket client) :
           stage(stage_name),
           total_players(total_players),
           stage_queue(QUEUE_SIZE),
           timer(stage_queue) {
  this->players.push_back(new Player(std::move(client)));
}

Game::~Game() {
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
  return (this->players.size() < this->total_players);
}

bool Game::ready() {
  return (this->players.size() == this->total_players);
}

void Game::add_player(Socket client) {
  if (this->players.size() >= this->total_players)
    return;

  printf("[Game] add_player -> new Player\n");
  this->players.push_back(new Player(std::move(client)));
  //TODO: init game? add worms to initiated game?
  //if (this->players.size() == this->total_players)
    //this->start();
}

void Game::prepare() {
  //set players in stage
  this->stage.set_worms_to_players(this->players.size());
  //crear colas bloqueantes de los jugadores
  int i = 0;
  for (auto& p : this->players) {
    p->set_id(i);
    i++;
    //printf("[Game] prepare -> new BlockingQueue\n");
    BlockingQueue<StageDTO>* q = new BlockingQueue<StageDTO>(QUEUE_SIZE);
    this->players_queues.push_back(q);
    p->add_stage_queues(q, &(this->stage_queue));
    //printf("[Game]starting player\n");
    p->start();
  }
  this->timer.start();

}

void Game::run() {
  // inicializar players, colas, timer
  prepare();
  // start game!
  StageDTO s;
  ActionDTO action;
  while (!this->stage.finished()) {
    // sacar action de la cola: action de player o action del timer(update)
    action = this->stage_queue.pop();
    //printf("[Game] pop action: %d\n", action.type);

    if (action.type == Quit) {
      //printf("[Game] end game\n");
      //end game: send block with endgame??
      this->stage.end();
      s.winner = -1;
      s.worm_turn = -1;
      this->timer.stop();
    } else {
      if (action.type == Timer_update){
        //printf("[Game] update state\n");
        this->stage.update();

      }
      else{
        //printf("[Game] le digo al stage que haga una accion\n");
        this->stage.make_action(action);

      }
      s = stage.get_stageDTO();
      //printf("[Game] pido un stage DTO\n");
    }
    for (auto &q : this->players_queues) {
      //printf("[Game] lo encolo en la cola de un player\n");
      q->push(s);
    }
  }
}

void Game::stop() {
  this->stage.end();
}
