#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <Box2D/Box2D.h>
#include "Constants.h"
#include "WeaponExplosionListener.h"
#include "Dtos.h"
#include "Worm.h"
#include "Beam.h"
#include "Stage.h"
#include "Weapon.h"
#include <random>


//config: yaml: https://github.com/jbeder/yaml-cpp/
Stage::Stage(std::string file_name) {
  std::cout << file_name << '\n';
  b2Vec2 gravity(0, Constants::gravity); //normal earth gravity
  this->world = new b2World(gravity);
  this->explosion_listener;
  this->world->SetContactListener(&this->explosion_listener);
  this->load_initial_stage(file_name);

  this->wind = Constants::wind;
}

Stage::~Stage() {
  //delete vectors (shouldn't use this, iterate over world's bodys)
  for (std::map<int, Worm*>::iterator it = this->worms.begin();
              it != this->worms.end(); ++it) {
    delete (it->second);
  }
  this->worms.clear();
  for (std::vector<Beam*>::iterator it = this->beams.begin();
            it != this->beams.end(); ++it) {
    delete (*it);
  }
  this->beams.clear();
  for (std::vector<Weapon*>::iterator it = this->explosions.begin();
          it != this->explosions.end(); ++it) {
    delete (*it);
  }
  this->explosions.clear();
  delete this->world;
}

void Stage::do_explosions() {
  //check for timers in explosion
  for (auto& w : this->explosions) {
    if (w->has_timer() && w->is_time_to_explode()) {
      w->explode();
    }
  }
}

void Stage::update() {
  float32 timeStep = Constants::time_step; //segundos del step
  int32 velocityIterations = Constants::velocity_iterations;   //how strongly to correct velocity
  int32 positionIterations = Constants::position_iterations;   //how strongly to correct position

  this->world->Step( timeStep, velocityIterations, positionIterations);

  //check for timers in explosion
  this->do_explosions();
  //delete weapons exploded and dead worms
  this->clean_dead_bodies();
  //check if player change
  this->update_player();

  //check falling worms
  this->current_player->update_state();
  //printf("curr state: %i\n", this->current_player->get_state());
}

bool Stage::finished() {
  return this->finish;
}

void Stage::end() {
  this->finish = true;
}

void Stage::clean_dead_bodies() {
  //delete "dead" explosions
  for (std::vector<Weapon*>::iterator it = this->explosions.begin();
      it != this->explosions.end();) {
     if(! (*it)->is_alive()) {
       //printf("it: %x", *it);
       delete *it;
       it = this->explosions.erase(it);
     } else {
       if ((*it)->has_timer() && (*it)->is_time_to_explode()) {
         (*it)->explode();
       }
       ++it;
     }
  }

  //delete dead worms
  std::map<int, Worm*>::iterator it = this->worms.begin();
  while (it != this->worms.end()) {
    if (!it->second->is_alive()) {
      if (it->second == this->current_player)
        this->change = true;
      this->players_turn.at(it->second->get_player_id()).delete_id(it->first);
      delete it->second;
      it = this->worms.erase(it);
    } else {
      ++it;
    }
  }

}

void Stage::update_player() {
  //printf("player time: %d\n", time(NULL) - this->player_time);
  if (this->change || (time(NULL) - this->player_time > Constants::worm_turn_time)) {
    printf("change player\n");
    this->current_player->took_weapon(None);
    this->change_player();
    this->change = false;
    this->player_time = time(NULL);
  }
}

//cuantos players puede haber????
void Stage::change_player() {
  int new_player_id = ((this->last_player_id + 1) == this->players_turn.size()) ? 0 : this->last_player_id + 1;
  printf("next player id: %d,", new_player_id);
  this->current_player = this->worms[this->players_turn.at(new_player_id).get_next()];
  printf(" worm id: %d\n", this->current_player->get_id());
  this->last_player_id = new_player_id;
}

void Stage::make_action(ActionDTO & action) {
  printf("%i, %i \n", action.type, action.move );
  int worm = action.worm_id;
  //VALIDAR TURNO!!
  if (worm != this->current_player->get_id()) {
    //throw Error("not current player: current: %d, sent: %d\n", this->current_player->get_id(), worm);
    return;
  }
  switch (action.type) {
    case (Make_move):{
      switch(action.move){
        case Walk_right:
          this->worms[worm]->move_right();
          break;
        case Walk_left:
          this->worms[worm]->move_left();
          break;
        case Jump:
          this->worms[worm]->jump(action.direction);
          break;
        case Jump_back:
          this->worms[worm]->jump_back();
          break;
      }
      break;
    }
    case(Take_weapon):{
      // cuando sea el fin del turno, asignar NONE al arma del gusano
      this->worms[worm]->took_weapon(action.weapon);
      break;

    }

    case(Shot_weapon):{
      oLog() << "Se disparo el arma al punto ("<< action.pos_x <<"," << action.pos_y <<") en metros,"//
      <<" con una potencia de" << action.power <<  "apuntando a"  << action.weapon_degrees <<" grados" //
      << "en la dire "<< action.direction <<" y con timer" << action.time_to_explode << "\n";

      //switch(action.weapon) {
      if (action.weapon == Teleport) {
        this->worms[worm]->teleport(action.pos_x, action.pos_y, action.direction);
      } else if (action.weapon == W_Air_Attack) {
        //TODO: fix : que no caigan todos juntos! (hace que exploten antes)
        for (int i = 0; i < 6; ++ i) {
          Weapon* w = new Weapon(this->world, action.weapon, action.pos_x, 0 - i, this->wind);
          this->explosions.push_back(w);
        }
      } else {
        //Weapon* w = new Weapon(this->world, action.weapon, this->current_player->get_points()[0].x, this->current_player->get_points()[0].y, this->wind);
        int d = (action.direction == Right) ? 1 : -1;
        Weapon* w = new Weapon(this->world, action.weapon, action.pos_x + d*3, action.pos_y , this->wind);
        w->shoot(action.power*100, action.weapon_degrees, action.direction, action.time_to_explode);
        this->explosions.push_back(w);
      }
      //TODO: esperar 3 segundos antes de cambiar el player
      this->change = true;
      //this->change_player();

      this->worms[worm]->change_state(Still);
      break;
    }
  }
}

void Stage::set_position(ElementDTO & element , b2Vec2 & center){
  element.pos_x = center.x;
  element.pos_y = center.y;

}

StageDTO Stage::get_stageDTO() {
  StageDTO s;
  for (auto w: this->worms) {
    ElementDTO worm_element;
    b2Vec2 center = w.second->get_center();
    set_position(worm_element, center);
    worm_element.player_id = w.second->get_player_id();
    worm_element.life = w.second->get_life();
    worm_element.angle = 0;
    worm_element.worm_state = w.second->get_state();
    worm_element.direction = w.second->get_direction();
    s.worms[w.first] = worm_element;
  }

  for (auto b: this->beams) {
    ElementDTO beam_element;
    b2Vec2 center = b->get_center();
    set_position(beam_element, center);
    std::vector<b2Vec2> vertices = b->get_points();
    beam_element.h = abs(vertices[0].y - vertices[2].y);
    beam_element.w = abs(vertices[0].x - vertices[1].x);
    beam_element.angle = b->get_angle();
    if ((beam_element.angle < 0.01) && (beam_element.angle > -0.01))
      beam_element.angle = 0;
    s.beams.push_back(beam_element);
  }

  for (auto w: this->explosions) {
    ElementDTO weapon;
    if (w->get_name() == None){
      continue;
    }
    if (w->get_name() == Explosion){
      w->explosion();
    }
    //std::vector<b2Vec2> vertices = w->get_points();
    //set_position(weapon, vertices);

    b2Vec2 center = w->get_center();
    set_position(weapon, center);
    weapon.weapon = w->get_name();
    weapon.timer = w->get_timer();
    s.weapons.push_back(weapon);
  }

  s.worm_turn = this->current_player->get_id();
  return s;
}

void Stage::load_initial_stage(std::string file_name){
  StageLoader yaml_loader(file_name);
  Stage_y s = yaml_loader.load_stage();
  oLog() << "loading initial stage:\n";
  for(auto b: s.beams){
    oLog() << "beam_y { x: " << b.pos_x << ", y: " << b.pos_y << ", size: " << b.size << ", inclination:" << b.inclination << "}" << endl;
	this->beams.push_back(new Beam(this->world, b.size, b.pos_x, b.pos_y, b.inclination));

  }
  for(auto & w: s.worms){
    oLog() << "worms: { id: "<< w.id << ", x: "<< w.pos_x << " , y: " << w.pos_y
       << ", direction: "<< w.direction << ", inclination: "<<w.inclination << ", life: " << w.life <<" }"<< endl;
    Worm* worm = new Worm(this->world, w.pos_x, w.pos_y, w.id, static_cast<Direction>( w.direction));
    this->worms.emplace(w.id, worm);
  }
}

void Stage::set_worms_to_players(int total_players) {
  //get vector of worms ids
  std::vector<int> ids;
  std::transform(this->worms.begin(), this->worms.end(), std::back_inserter(ids),
            [](const std::pair<int,Worm*>& val){return val.first;});
  //and shuffle it
  auto rng = std::default_random_engine {};
  std::shuffle(std::begin(ids), std::end(ids), rng);
  //create turn helpers
  int wq = this->worms.size() / total_players;
  if(wq < 0){
    throw Error("Error in worms quantity:\nplayers = %d"//
      ", total worms = %i", total_players, this->worms.size());
  }
  printf("total players: %d, worms for each: %d\n", total_players, wq);
  for (int i = 0; i < total_players; i++) {
    //(i+1)*wq == ids.size()) ? ids.end() : ids[(i+1)*wq])
    std::vector<int> v;
    std::copy(ids.begin() + i*wq, ids.begin() + (i+1)*wq, std::back_inserter(v));
    this->players_turn.emplace(i, TurnHelper(v, i));
  }
  //compensar jugador con menos gusanos!!
  this->current_player = this->worms[0];
}
