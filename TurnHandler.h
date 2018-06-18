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

class TurnHandler {
	int current_player_turn;
	Worm * current_worm_turn;
public:
	TurnHandler(int total_players, std::map<int, Worm*> &worms);
	virtual ~TurnHandler();
};

#endif /* TURNHANDLER_H_ */
