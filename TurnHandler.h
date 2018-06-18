/*
 * TurnHandler.h
 *
 *  Created on: 18 jun. 2018
 *      Author: jaz
 */

#ifndef TURNHANDLER_H_
#define TURNHANDLER_H_
#include <map>
#include <vector>
#include "Worm.h"
#include "Error.h"
#include "PlayerTurn.h"

class TurnHandler {
	int current_player_turn;
	Worm * current_worm_turn;
	std::map<int, PlayerTurn> players_turn;
public:
	TurnHandler(int total_players, std::map<int, Worm*> &worms);
	void delete_worm(int player_id, int worm_id);
	virtual ~TurnHandler();
};

#endif /* TURNHANDLER_H_ */
