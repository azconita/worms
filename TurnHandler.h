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
#include "Constants.h"

class TurnHandler {
	int current_player_turn;
	int last_player_id;
	int current_worm_turn;
	time_t player_time = time(NULL);
	//std::map<int, Worm*> & all_worms;
	std::map<int, PlayerTurn> players_turn;
	bool ready_to_change;


	int get_player_with_less_worms();
	void change_worms_states(std::map<int, Worm*> & worms);
	bool is_any_in_movement(std::map<int, Worm*> & worms);
	void change_player(std::map<int, Worm*> & worms);
public:
	TurnHandler(int total_players, std::map<int, Worm*> & worms);
	int get_worm_turn_id();
	void delete_worm(int player_id, int worm_id,std::map<int, Worm*> & worms);
	void ready_to_change_player();
	bool update_player(std::map<int, Worm*> & worms);
	virtual ~TurnHandler();
};

#endif /* TURNHANDLER_H_ */
