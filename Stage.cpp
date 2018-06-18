
#include "Stage.h"  

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
      this->turnHandler->ready_to_change_player();
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

  //update worms: set vel 0 for "stopped" worms and static
  this->update_worms();

  //check falling worms
  this->worms.at(this->turnHandler->get_worm_turn_id())->update_state();

  //check if player change
  this->turnHandler->update_player(this->worms);

  
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
      this->turnHandler->delete_worm(it->second->get_player_id(), it->first,this->worms);
      delete it->second;
      it = this->worms.erase(it);
    } else {
      ++it;
    }
  }

}



/**
   Update the velocity of the worms
   and made teleport effective
*/
void Stage::update_worms() {
  for (auto &w : this->worms) {
    if(w.second->disappear()){
      this->turnHandler->ready_to_change_player();
    }
    if(w.second->get_velocity().Length() > 0.3){
    } else{
       w.second->stop_moving();
    }
    
   }

 }




void Stage::make_action(ActionDTO & action) {
  int worm = action.worm_id;
  printf("[Stage] accion de %i, turno de %i\n",worm, this->turnHandler->get_worm_turn_id() );
  if (worm != this->turnHandler->get_worm_turn_id()) {
    printf("no es tu turno\n");
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
          if (this->worms[worm]->get_state()==Still)
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
        this->worms[worm]->teleport(action.pos_x, action.pos_y);
      } else if (action.weapon == W_Air_Attack) {
        //TODO: fix : que no caigan todos juntos! (hace que exploten antes)
        for (int i = 0; i < 6; ++ i) {
          Weapon* w = new Weapon(this->world, action.weapon, action.pos_x, 0 - i, this->wind);
          this->explosions.push_back(w);
        }
      } else {
        //Weapon* w = new Weapon(this->world, action.weapon, this->current_player->get_points()[0].x, this->current_player->get_points()[0].y, this->wind);
        int d = (action.direction == Right) ? 1 : -1;
        printf("[Stage] posicion del arma%i, %i\n",action.pos_x, action.pos_y );
        Weapon* w = new Weapon(this->world, action.weapon, action.pos_x , action.pos_y , this->wind);
        w->shoot(action.power*100, action.weapon_degrees, action.direction, action.time_to_explode);
        this->explosions.push_back(w);
      }
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
    set_position(weapon, center);
    weapon.weapon = w->get_name();
    weapon.timer = w->get_timer();
    printf("[Stage] timer sent: %i\n", weapon.timer);
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
	this->turnHandler = new TurnHandler(total_players, this->worms);
}


Stage::~Stage() {
	delete(this->turnHandler);
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
