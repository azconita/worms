/*
 * TurnHandler.cpp
 *
 *  Created on: 18 jun. 2018
 *      Author: jaz
 */

#include "TurnHandler.h"

TurnHandler::TurnHandler(int total_players, std::map<int, Worm*> &worms) {

	//get vector of worms ids
	  std::vector<int> worms_ids;
	  std::transform(worms.begin(), worms.end(), std::back_inserter(worms_ids),
	            [](const std::pair<int,Worm*>& val){return val.first;});

	  //and shuffle it
	  auto rng = std::default_random_engine {};
	  std::shuffle(std::begin(worms_ids), std::end(worms_ids), rng);

	  //create turn helpers
	  int worms_per_player = worms.size() / total_players;
	  if(worms_per_player < 0){
	    throw Error("Error in worms quantity:\nplayers = %d"//
	      ", total worms = %i", total_players, worms.size());
	  }
	  printf("[TurnHandler] total players: %d, worms for each: %d\n", total_players, worms_per_player);

	  for (int i = 0; i < total_players; i++) {
	    std::vector<int> v;
	    std::copy(worms_ids.begin() + i*worms_per_player, worms_ids.begin() + (i+1)*worms_per_player, std::back_inserter(v));
	    this->players_turn.emplace(i, PlayerTurn(v, i));
	    for (int i: v) {
	      worms.at(i)->set_player_id(i);
	    }
	  }

	  //compensar jugador con menos gusanos!!
	  this->current_worm_turn = worms[0];

}

void TurnHandler::delete_worm(int player_id, int worm_id){
	this->players_turn.at(player_id).delete_worm_id(worm_id);
}

TurnHandler::~TurnHandler() {
	// TODO Auto-generated destructor stub
}

