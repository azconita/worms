/*
 * Game.h
 *
 *  Created on: Jun 4, 2018
 *      Author: gabi
 */

#ifndef GAME_H_
#define GAME_H_

#include <string>
#include "common_socket.h"
#include "Constants.h"
#include "DTOs.h"
#include "Player.h"
#include "Stage.h"

class Game {
  //thread!!
  Stage stage;
  std::vector<Player> players;
  int limit = Constants::players_limit;
public:
  Game(std::string &stage_name, Socket &client);
  virtual ~Game();

  bool not_full();
  void add_player(Socket client);

  void start();

  std::string get_yaml(StageDTO &s);
};

#endif /* GAME_H_ */
