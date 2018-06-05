#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <Box2D/Box2D.h>
#include "Constants.h"
#include "WeaponExplosionListener.h"
#include "DTOs.h"
#include "Worm.h"
#include "Beam.h"
#include "Stage.h"
#include "Weapon.h"

//config: yaml: https://github.com/jbeder/yaml-cpp/
Stage::Stage(std::string file_name) {
  std::cout << file_name << '\n';
  b2Vec2 gravity(0, Constants::gravity); //normal earth gravity

  this->world = new b2World(gravity);
  this->explosion_listener;
  this->world->SetContactListener(&this->explosion_listener);
  this->load_initial_stage(file_name);
  this->current_player = this->worms.begin()->second;
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
  printf("delete world!\n");
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
  //char t = (this->worms[0]->is_falling()) ? 'y' : 'n';
  //printf("worm falling: %c\n", t);
}

bool Stage::finished() {

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
      delete it->second;
      it = this->worms.erase(it);
    } else {
      ++it;
    }
  }

}

void Stage::update_player() {
  printf("player time: %d\n", (difftime(this->player_time,time(NULL))) );
  if (this->change || (difftime(this->player_time,time(NULL)) > 60)) {
    printf("change player\n");
    this->change_player();
    this->change = false;
  }
}

//TODO sirve así? gusanos mueren... de quien es el turno?
//cuantos players puede haber????
void Stage::change_player() {
  int curr_id = this->current_player->get_id();
  std::map<int, Worm*>::iterator next = this->worms.find(curr_id);
  if (++next == this->worms.end())
    this->current_player = this->worms.begin()->second;
  else
    this->current_player = next->second;
  time(&(this->player_time));
}

void Stage::make_action(ActionDTO & action) {
  int worm = action.worm_id;
  //VALIDAR TURNO!!
  if (worm != this->current_player->id)
    return;//lanzar excepcion o algo: gusano invalido!
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
      printf("Se tomo el arma %i\n", action.weapon);
      // cuando sea el fin del turno, asignar NONE al arma del gusano
      this->worms[worm]->took_weapon(action.weapon);
      break;

    }

    case(Shot_weapon):{
      printf("Se disparo el arma al punto (%i,%i) en metros, con una potencia de %i apuntando a %f grados en la dire %i y con timer %i\n",//
       action.x, action.y, action.power, action.weapon_degrees, action.direction, action.time_to_explode);

      //switch(action.weapon) {
      if (action.weapon == Teleport) {
        this->worms[worm]->teleport(action.x, action.y, action.direction);
      } else if (action.weapon == W_Air_Attack) {
        //TODO: fix : que no caigan todos juntos! (hace que exploten antes)
        for (int i = 0; i < 6; ++ i) {
          Weapon* w = new Weapon(this->world, action.weapon, action.x, 0, this->wind);
          this->explosions.push_back(w);
        }
      } else {
        Weapon* w = new Weapon(this->world, action.weapon, action.x, action.y, this->wind);
        printf("about to shoot");
        w->shoot(action.power, action.weapon_degrees, action.direction, action.time_to_explode);
        this->explosions.push_back(w);
      }
      //TODO: esperar 3 segundos antes de cambiar el player
      this->change = true;
      //this->change_player();
      break;
    }
  }
}

void Stage::set_position(ElementDTO & element , std::vector<b2Vec2> & vertices){
  b2Vec2 up_left = vertices[0];
  b2Vec2 down_right = vertices[2];


  element.x = up_left.x;
  element.y = up_left.y;

  if(down_right.y > up_left.y){
    element.h = (down_right.y - up_left.y);
  }else{
    element.h = (up_left.y - down_right.y);
  }
  if(down_right.x > up_left.x){
    element.w = (down_right.x - up_left.x);
  }else{
    element.w = (up_left.x - down_right.x);
  }
}

StageDTO Stage::get_stageDTO() {
  StageDTO s;
  for (auto w: this->worms) {
    ElementDTO worm_element;
    std::vector<b2Vec2> vertices = w.second->get_points();
    set_position(worm_element, vertices);
    worm_element.player_id = w.second->get_player_id();
    worm_element.life = w.second->get_life();
    //printf("worm %i: x = %f y = %f  h = %f w = %f\n",w.first, worm_element.x, worm_element.y, worm_element.h, worm_element.w);
    s.worms[w.first] = worm_element;
  }

  for (auto b: this->beams) {
    ElementDTO beam_element;
    std::vector<b2Vec2> vertices = b->get_points();
    set_position(beam_element, vertices);
    //printf("beam : x = %f y = %f  h = %f w = %f\n", beam_element.x, beam_element.y, beam_element.h, beam_element.w);
    beam_element.angle = b->get_angle();
    //printf("beam angle: %d\n", beam_element.angle);
    s.beams.push_back(beam_element);
  }

  for (auto w: this->explosions) {
    ElementDTO weapon;
    if (w->get_name() == None)
      continue;
    if (w->get_name() == Explosion)
      w->explosion();
    //std::vector<b2Vec2> vertices = w->get_points();
    //set_position(weapon, vertices);

    b2Vec2 point = w->get_point();
    weapon.x = point.x;
    weapon.y = point.y;
    weapon.h = 0.5;
    weapon.w = 0.5;
    weapon.weapon = w->get_name();
    weapon.timer = w->get_timer();
    s.weapons.push_back(weapon);
  }

  s.worm_turn = 0;
  //s.worm_turn = this->current_player->get_id();
  return s;
}



void Stage::load_initial_stage(std::string file_name){
  StageLoader yaml_loader(file_name);
  Stage_y s = yaml_loader.load_stage();
   for(auto b: s.beams){
    cout << "beam_y { x: " << b.pos_x << ", y: " << b.pos_y << ", size: " << b.size << ", inclination:" << b.inclination << "}" << endl;
     this->beams.push_back(new Beam(this->world,  b.pos_x, b.pos_y, b.inclination));
  }
  for(auto & pair: s.players){
    cout << "player  " << pair.first << endl;
    cout << "worms: ";
    for(auto w : pair.second){
      cout << "{ id: "<< w.id << ", x: "<< w.pos_x << " , y: "
      << w.pos_y << ", direction: "<< w.direction << ", inclination: "<<w.inclination << ", life: " << w.life <<" }"<< endl;
      Worm* worm = new Worm(this->world, w.pos_x, w.pos_y, w.id);
      this->worms.emplace(0, worm);

    }
  }
}


/*// set initial stage
void Stage::add_beams(std::string config) {

  this->beams.push_back(new Beam(this->world, 10,20));
  this->beams.push_back(new Beam(this->world, 20,20));
  this->beams.push_back(new Beam(this->world, 30,20));

  this->beams.push_back(new Beam(this->world, 30,40,0));
}

void Stage::add_worms(std::string config) {
  Worm* w = new Worm(this->world, 10, 11);
  this->worms.emplace(0, w);
}

void Stage::add_weapons(std::string config) {

}

void Stage::add_explosion() {
  //this->explosions.push_back(new Projectile(this->world, W_Bazooka, 9, 12));
}*/
