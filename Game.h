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
#include "CommonThread.h"
#include "Socket.h"
#include "Constants.h"
#include "Dtos.h"
#include "Player.h"
#include "Stage.h"
#include "TimerStage.h"
#include "Logger.h"

extern  logger oLog;

#define QUEUE_SIZE 100

class Game : public Thread {
  Stage stage;
  TimerStage timer;
  std::vector<Player*> players;
  int total_players;
  int limit = Constants::total_players;
  BlockingQueue<ActionDTO> stage_queue;
  std::vector<BlockingQueue<StageDTO>*> players_queues;

  bool stopped = false;
  bool ran = false;

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
