/*
 * Game.h
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#ifndef GAME_H_
#define GAME_H_

#include <string>
#include <vector>

#include "BlockingQueue.h"
#include "Socket.h"
#include "common_thread.h"
#include "Constants.h"
#include "Dtos.h"
#include "Player.h"
#include "Stage.h"
#include "TimerStage.h"
#define QUEUE_SIZE 100

class Game : public Thread {
  //thread!!
  Stage stage;
  TimerStage timer;
  std::vector<Player*> players;
  int total_players;
  int limit = Constants::total_players;
  BlockingQueue<ActionDTO> stage_queue;
  std::vector<BlockingQueue<StageDTO>*> players_queues;

  bool stopped = false;

public:
  Game(std::string &stage_name, int total_players, Socket client);
  ~Game();

  bool not_stopped();
  bool not_full();
  bool ready();
  void add_player(Socket client);

  virtual void run() override ;
  void stop();
private:
  void prepare();

};

#endif /* GAME_H_ */
