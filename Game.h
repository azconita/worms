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
#include "common_socket.h"
#include "common_thread.h"
#include "Constants.h"
#include "DTOs.h"
#include "Player.h"
#include "Stage.h"
#include "TimerStage.h"

class Game : public Thread {
  //thread!!
  Stage stage;
  std::vector<Player> players;
  int limit = Constants::players_limit;
  BlockingQueue<ActionDTO> stage_queue;
  std::vector<BlockingQueue<StageDTO>*> players_queues;

  TimerStage timer;


public:
  Game(std::string &stage_name, Socket &client);
  virtual ~Game();

  bool not_full();
  void add_player(Socket client);

  virtual void run() override ;

private:
  void prepare();
  //std::string get_yaml(StageDTO &s);
};

#endif /* GAME_H_ */
