/*
 * TurnHandler.cpp
 *
 *  Created on: 18 jun. 2018
 *      Author: jaz
 */

#include "TurnHandler.h"

TurnHandler::TurnHandler(int total_players, std::vector<int> worms_ids){
	  //and shuffle it
	  auto rng = std::default_random_engine {};
	  std::shuffle(std::begin(worms_ids), std::end(worms_ids), rng);

	  //create turn helpers
	  int worms_per_player = worms_ids.size() / total_players;
	  if(worms_per_player < 0){
	    throw Error("Error in worms quantity:\nplayers = %d"//
	      ", total worms = %i", total_players, worms.size());
	  }
	  printf("[TurnHandler] total players: %d, worms for each: %d\n", total_players, worms_per_player);

	  for (int i = 0; i < total_players; i++) {
	    std::vector<int> v;
	    std::copy(worms_ids.begin() + i*worms_per_player, worms_ids.begin() + (i+1)*worms_per_player, std::back_inserter(v));
	    this->players_turn.emplace(i, PlayerTurn(v, i));
	    for (int worm_pos: v) {
	      worms.at(worm_pos)->set_player_id(i);
	    }
	  }
	  int less = this->get_player_with_less_worms();
	  this->current_player_turn = less;
	  this->last_player_id = less;
	  this->current_worm_turn = this->players_turn.at(less).get_next_worm_id();
	  printf("[TurnHandler] empieza con turno del jugador %i, con su gusano %i", this->current_player_turn, this->current_player_turn);
	  this->ready_to_change = false;
	  this->player_time = time(NULL);

}

int TurnHandler::get_player_with_less_worms(){
	int less = 0;
	for(int i = 0;i < this->players_turn.size(); i++){
		if(this->players_turn.at(i).get_worms_size() < this->players_turn.at(less).get_worms_size() ){
			less = i;
		}
	}
	return less;
}


void TurnHandler::delete_worm(int player_id, int worm_id){
	if(this->current_worm_turn == worm_id){
		this->change_player();
	}
	this->players_turn.at(player_id).delete_worm_id(worm_id);
}

void TurnHandler::ready_to_change_player(){
	this->ready_to_change = true;
}



bool TurnHandler::update_player(bool in_movement) {
  if (this->ready_to_change  ||  (time(NULL) - this->player_time > Constants::worm_turn_time)  && !in_movement){
    printf("[TurnHandler] change player\n");
    this->change_player();
    this->ready_to_change = false;
    this->player_time = time(NULL);
    return true;
  }
  return false;
}


void TurnHandler::change_player(){
  this->current_player_turn =  ((this->last_player_id + 1) == this->players_turn.size()) ? 0 : this->last_player_id + 1;
  printf("[TurnHandler] next player id: %d\n", this->current_player_turn);
  this->current_worm_turn = this->players_turn.at(this->current_player_turn).get_next_worm_id();
  printf("[TurnHandler] worm id: %d\n", this->current_worm_turn);
  this->last_player_id = this->current_player_turn;
}

int TurnHandler::get_worm_turn_id(){
	return this->current_worm_turn;
}

TurnHandler::~TurnHandler() {
	// TODO Auto-generated destructor stub
}

