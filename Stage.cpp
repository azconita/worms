
#include "Stage.h"

//config: yaml: https://github.com/jbeder/yaml-cpp/
Stage::Stage(std::string file_name) : ammo() {
  std::cout << file_name << '\n';
  b2Vec2 gravity(0, Constants::gravity); //normal earth gravity
  this->world = new b2World(gravity);
  this->explosion_listener;
  this->world->SetContactListener(&this->explosion_listener);
  this->load_initial_stage(file_name);

  this->wind = Constants::wind;
}

void Stage::do_explosions() {
  //check for timers in explosion
  for (auto& w : this->explosions) {
    if (w->has_timer() && w->is_time_to_explode()) {
      //todos los gusanos dinamicos antes de la explosion
       std::map<int, Worm*>::iterator it = this->worms.begin();
        while (it != this->worms.end()) {
            it->second->set_dynamic();
            ++it;
        }
      w->explode();
      printf("[Stage] se hizo una explosion, listo para cambiar de jugaro\n");
      this->current_player->took_weapon(None);
      //this->current_player = NULL;
    }
  }
}


void Stage::update() {
  float32 timeStep = Constants::time_step; //segundos del step
  int32 velocityIterations = Constants::velocity_iterations;   //how strongly to correct velocity
  int32 positionIterations = Constants::position_iterations;   //how strongly to correct position

  //printf("[Stage] hace un step\n");

  this->world->Step( timeStep, velocityIterations, positionIterations);

  //printf("[Stage] check for explosiones\n");
  //check for timers in explosion
  this->do_explosions();

  //printf("[Stage] borra gusanos muertos\n");
  //delete weapons exploded and dead worms
  this->clean_dead_bodies();

  //printf("[Stage] update del player\n");

  //check if player change
  this->update_player();
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
       printf("[Stage]borrando armaaaaaaaaaaa\n");
       if(this->explosions.size() == 0){
          printf("[Stage] listo para cambiar de jugador\n");
          this->current_player = NULL;
       }
     } else {
       if ((*it)->has_timer() && (*it)->is_time_to_explode()) {
         (*it)->explode();
         for (auto &w : this->worms) {
               w.second->set_dynamic();
          }
       }
       ++it;
     }
  }

  //delete dead worms
  std::map<int, Worm*>::iterator it = this->worms.begin();
  while (it != this->worms.end()) {
    if (!it->second->is_alive()) {
      if (it->second == this->current_player)
        this->current_player = NULL;
        //printf("[Stage] se murio un gusano, listo para cambiar de jugador\n");
      this->players_turn.at(it->second->get_player_id()).delete_id(it->first);
      delete it->second;
      it = this->worms.erase(it);
    } else {
      ++it;
    }
  }

}

void Stage::update_player() {
    //update worms: set vel 0 for "stopped" worms and static
  bool any_worm_in_movement = this->is_in_movement(); 
  if (this->current_player == NULL || ( (time(NULL) - this->player_time > Constants::worm_turn_time) && this->explosions.size() == 0) ) {
    if (this->current_player == NULL)
      printf("NULL player\n");
    if(any_worm_in_movement){
      printf("[Stage] queria cambiar de jugador pero se estan moviendo entonces sigue sin turno\n");
      return;
    }
    printf("[Stage] change player\n");
    this->change_player();
    this->player_time = time(NULL);
  }
}

bool Stage::is_in_movement(){
  if(this->update_worms() == true){
    return true;
  }
  for(auto &w: this->weapons){
    if(w->get_velocity().Length() > 0.5){
      return true;
    }
  }
  return false;
}


/**
   Update the velocity of the worms
  made teleport effective
  devuelve true si hay algun gusano moviendose
  sino false
*/
bool Stage::update_worms() {
  bool any_worm_in_movement = false;
  for (auto &w : this->worms) {
    if(w.second->disappear()){
      //printf("[Stage] el gusano ya se teletransporto, esta listo para cambiar de jugador\n");
      this->current_player = NULL;
    }
    w.second->update_state();
    if(w.second->get_velocity().Length() > 0.5){
      // si los gusanos se estan moviendo quizas es por una explosion
      //no hay que cambiar de turno porque se vuelven estaticos
      any_worm_in_movement = true;
    } else {
       w.second->stop_moving();
    }
  }
  return any_worm_in_movement;
}

//cuantos players puede haber????
void Stage::change_player() {
  int new_player_id;

  int step = 0;
  while(step < this->players_turn.size()){
      step++;
      new_player_id = ((this->last_player_id +1) == this->players_turn.size() ) ? 0 : (this->last_player_id +1);
      this->last_player_id = new_player_id;
      printf("new player %i\n", new_player_id);

      if(this->players_turn.find(new_player_id) == this->players_turn.end() || this->players_turn.at(new_player_id).is_empty()){
          printf("[Stage] jugador %d  estaba muerto, pasamos al siguiente\n", new_player_id);
          continue; //el jugador no tiene gusanos o fue eliminado de la partida
      } 
      break;  
  }
  
  if(this->players_turn.find(new_player_id) == this->players_turn.end() || this->players_turn.at(new_player_id).is_empty()){   
      printf("[Stage] todos los jugadores estab muertos\n,");
      new_player_id = -2;
      return;
  }

  this->current_player = this->worms[this->players_turn.at(new_player_id).get_next()];
  printf("worm id: %d\n", this->current_player->get_id());
  this->update_body_types(false);
}



void Stage::update_body_types(bool first_time){
if(this->current_player == NULL){
  throw Error("No se puede cambiar el body type si no es el turno de nadie");

}
  for (auto &w : this->worms) {

    w.second->took_weapon(None);

   if (w.first != this->current_player->get_id()){
       w.second->set_static(first_time);

     }else{
       w.second->set_dynamic();
     }
   }
 }




void Stage::shoot_weapon(int worm, ActionDTO& action) {

  oLog() << "Se disparo el arma al punto ("<< action.pos_x <<"," << action.pos_y <<") en metros,"//
        <<" con una potencia de" << action.power <<  "apuntando a"  << action.weapon_degrees <<" grados" //
        << "en la dire "<< action.direction <<" y con timer" << action.time_to_explode << "\n";

  if (action.weapon == Teleport) {
    this->worms[worm]->teleport(action.pos_x, action.pos_y);

  } else if (action.weapon == W_Air_Attack) {
    for (int i = 0; i < 6; ++i) {
      printf("new air bomb\n");
      Weapon* w = new Weapon(this->world, action.weapon, action.pos_x, 0 - i,
          this->wind, &this->explosions);
      this->explosions.push_back(w);
    }

  } else {
    //Weapon* w = new Weapon(this->world, action.weapon, this->current_player->get_points()[0].x, this->current_player->get_points()[0].y, this->wind);
    int d = (action.direction == Right) ? 1 : -1;
    //printf("[Stage] posicion del arma%i, %i\n", action.pos_x, action.pos_y);
    Weapon* w = new Weapon(this->world, action.weapon, action.pos_x +3*d, action.pos_y -3, this->wind, &this->explosions);
    w->shoot(action.power * 100, action.weapon_degrees, action.direction,
        action.time_to_explode);
    //this->worms[action.worm_id]->took_weapon(None);
    this->explosions.push_back(w);
  }
  this->worms[worm]->took_weapon(None);
}

void Stage::worm_make_move(int worm, ActionDTO& action) {
  switch (action.move) {
    case Walk_right:
      this->worms[worm]->move_right();
      break;
    case Walk_left:
      this->worms[worm]->move_left();
      break;
    case Jump:
      State current_state = this->worms[worm]->get_state();
      if ( current_state == Still || current_state == Still_up || current_state == Still_down){
        this->worms[worm]->jump(action.direction);
      }
      break;
    case Jump_back:
      State current_state = this->worms[worm]->get_state();
      if ( current_state == Still || current_state == Still_up || current_state == Still_down){
        this->worms[worm]->jump_back();
      }
      break;
  }
}

void Stage::make_action(ActionDTO & action) {
  int worm = action.worm_id;
  //VALIDAR TURNO!!
  if (worm != this->current_player->get_id()) {
    //throw Error("not current player: current: %d, sent: %d\n", this->current_player->get_id(), worm);
    return;
  }
  switch (action.type) {
    case (Make_move):{
      worm_make_move(worm, action);
      break;
    }

    case(Take_weapon):{
      if (this->explosions.empty())
      // cuando sea el fin del turno, asignar NONE al arma del gusano
        this->worms[worm]->took_weapon(action.weapon);
      break;
    }

    case(Shot_weapon):{
      if (this->explosions.empty())
        shoot_weapon(worm, action);
      break;
    }
  }
}

void Stage::set_position(ElementDTO & element , b2Vec2 & center){
  element.pos_x = center.x;
  element.pos_y = center.y;

}

int Stage::check_winners(){
  for (auto it = this->players_turn.cbegin(); it != this->players_turn.cend(); ) {
    if (it->second.is_empty()){
      printf("[Stage]  LOSER %i\n", it->first);
      it = this->players_turn.erase(it++); 
      if(this->players_turn.size() == 1){
        printf("[Stage] WINNER %i\n", this->players_turn.begin()->first );
        //this->finish = true;
        return this->players_turn.begin()->first;
      }
    } else {
      ++it;
    }
  }
  return -1;
}

StageDTO Stage::get_stageDTO() {
  StageDTO s;
  s.winner = this->check_winners();
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
    std::vector<b2Vec2> v = b->get_points();

    beam_element.h = round(sqrt(pow(v[2].x - v[1].x,2) + pow(v[2].y - v[1].y,2)));
    beam_element.w = round(sqrt(pow(v[1].x - v[0].x,2) + pow(v[1].y - v[0].y,2)));
    beam_element.angle = b->get_angle();
    beam_element.direction = b->get_direction();
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
    b2Vec2 center = w->get_center();
    weapon.angle = w->get_angle_velocity();
    set_position(weapon, center);
    weapon.weapon = w->get_name();
    weapon.timer = w->get_timer();
    ////printf("[Stage] timer sent: %i\n", weapon.timer);
    if (weapon.weapon == W_Fragment)
      printf("fragment: pos: %f, %f\n", weapon.pos_x, weapon.pos_y);
    s.weapons.push_back(weapon);
  }
  s.worm_turn = (this->current_player) ? this->current_player->get_id() : INTER_TURN;
  //printf("[Stage] envia el dto\n");
  return s;
}

void Stage::load_initial_stage(std::string file_name){
  StageLoader yaml_loader(file_name);
  Stage_y s = yaml_loader.load_stage();
  oLog() << "loading initial stage:\n";
  for(auto b: s.beams){
    oLog() << "beam_y { x: " << b.pos_x << ", y: " << b.pos_y << ", size: " << b.size
    << ", inclination:" << b.inclination << ", direction: " << b.direction <<"}" << endl;
	this->beams.push_back(new Beam(this->world, b.size, b.pos_x, b.pos_y, b.inclination,static_cast<Direction>(b.direction)));

  }
  for(auto & w: s.worms){
    oLog() << "worms: { id: "<< w.id << ", x: "<< w.pos_x << " , y: " << w.pos_y
       << ", direction: "<< w.direction << ", inclination: "<<w.inclination << ", life: " << w.life <<" }"<< endl;
    Worm* worm = new Worm(this->world, w.pos_x, w.pos_y, w.id, static_cast<Direction>( w.direction));
    this->worms.emplace(w.id, worm);
  }
}

void Stage::set_worms_to_players(int total_players) {
  //printf("[Stage] cantidad de gusanos: %lu\n",this->worms.size() );
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
  //printf("[Stage] total players: %d, worms for each: %d\n", total_players, wq);
  for (int i = 0; i < total_players; i++) {
    printf("[Stage] set for player %i\n", i);
    //(i+1)*wq == ids.size()) ? ids.end() : ids[(i+1)*wq])
    std::vector<int> v;
    std::copy(ids.begin() + i*wq, ids.begin() + (i+1)*wq, std::back_inserter(v));
    this->players_turn.emplace(i, TurnHelper(v, i));
    for (auto worm_id: v) {
      this->worms.at(worm_id)->set_player_id(i);
      //printf("[Stage] al gusano %i le corresponde el jugador %i \n", worm_id, i);
    }
  }

  //compensar jugador con menos gusanos!!
  this->current_player = this->worms.begin()->second;
  printf("[Stage] empieza a jugar el gusano %i \n", this->current_player->get_id() );
  this->last_player_id = this->current_player->get_player_id();
  printf(" que es del jugador %i\n", this->last_player_id );
  this->update_body_types(true);
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
  printf("[Stage] deleted\n");
}
