/*
 * Worm.cpp
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include "Worm.h"
#include "Constants.h"
#include <string>
#include <cmath>



Worm::Worm(b2World* world, float x, float y, int id, Direction direction) :
          Entity(1), world(world), id(id), ammo() {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);
  bodyDef.bullet = false;
  bodyDef.userData = (void*) this;
  this->body = world->CreateBody(&bodyDef);
  std::cout << "wormDir: " << this << '\n';
  //add box fixture
  b2PolygonShape shape;
  shape.SetAsBox(Constants::worm_width, Constants::worm_height);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = Constants::worm_density;
  myFixtureDef.friction = Constants::worm_friction;
  this->body->CreateFixture(&myFixtureDef);
  this->body->SetUserData(this);
  this->life = Constants::worm_initial_life;
  this->state = Still;
  this->direction = direction;
  this->inclination = 0;
}

Worm::Worm(const Worm& other) : Entity(1),
                                body(other.body),
                                life(other.life),
                                world(world),
                                player_id(other.player_id),
                                direction(other.direction),
                                state(other.state),
                                id(other.id),
                                inclination(inclination),
                                weapon(other.weapon) {
  this->body->SetUserData(this);
  std::cout << "wormDir(&other): " << this << '\n';
}

/*Worm::Worm() : Entity(1), body(NULL), life(0), world(NULL) {
  std::cout << "wormDir(): " << this << '\n';

}*/

Worm::~Worm() {
  // TODO Auto-generated destructor stub
  std::cout << "worm destroyed " << '\n';
  this->world->DestroyBody(this->body);
}


Worm* Worm::operator=(const Worm &other) {
  std::cout << "wormDir=: " << this << '\n';
  this->body = other.body;
  this->life = other.life;
  this->state = other.state;
  this->direction = other.direction;
  this->inclination = other.inclination;
  this->state = state;
  this->body->SetUserData(this);
  return this;
}

float Worm::get_impulse() {
  b2Vec2 vel = body->GetLinearVelocity();
  float velChange = Constants::worm_walk_velocity - vel.x;
  return body->GetMass() * velChange;
}

State Worm::get_state(){
  return this->state;
}

void Worm::change_state(State state){
  this->state = state;
}

/*
 * actualiza estado del gusano.
 * @return:
 * si el gusano estaba cayendo y recibio dano por la caida, devuelve true
 * si no, devuelve false.
 */
bool Worm::update_state() {
  bool is_worm_damaged = this->update_horizontal_state();
  if(this->inclination != 0 && this->inclination != 180){
    if(this->inclination < 90){
      printf("busca estado inclinado\n");
      this->state = (this->direction == Right)? down_states.at(this->state) :up_states.at(this->state);
    }else if(this->inclination > 90){
      this->state = (this->direction == Right)? up_states.at(this->state) :down_states.at(this->state);
    }
  }
  return is_worm_damaged;
}

bool Worm::update_horizontal_state(){
  //caer es el estado "predominante"
  if(this->state == Worm_disappear){
    return false;
  }
  this->handle_end_contact();
  b2Vec2 vel = this->body->GetLinearVelocity();
  if (vel.y > 5 && this->state != Fall && this->inclination == 0) {
      this->state = Fall;
      this->start_falling = this->body->GetPosition();
      oLog() << "[Worm] start falling: "<< this->start_falling.x << " "<<this->start_falling.y << endl;
      return false;
  
  }
  if (std::abs(vel.y) < 1 && std::abs(vel.x) < 1 && this->state == Fall) {
    //estaba cayendo: chequear si fueron mas de 2m para hacerle dano al worm
    this->state = Still;
    float d = std::abs(this->start_falling.y - this->body->GetPosition().y);
    oLog() << "[Worm] worm stops falling: " << d << endl;
    if (d > 2)
      this->apply_damage((d < 25) ? d : 25);
    return true;
  }
  if (this->state != Still && (std::abs(vel.y) < 0.1 && std::abs(vel.x) < 0.1 )
     && (this->state == Walk || this->state == Jump_state || this->state == Jump_back_state
      || this->state == Fall) || this->state == Walk_down || this->state == Walk_up) {
    this->state = Still;
    //printf("still");
    return false;
  }
  return false;

}

Direction Worm::get_direction(){
  return this-> direction;
}

void Worm::calm(){
  if(this->state == Still_up || this->state == Walk_up){
            change_state(Still_down);
            return;
  }
  if(this->state == Still_down || this->state == Walk_down){
            change_state(Still_up);
            return;
  } 
  change_state(Still);

}

void Worm::move(float vel_x, float vel_y){
  printf("velocidad: %f, %f\n",vel_x, vel_y);
  this->body->ApplyLinearImpulse(b2Vec2(vel_x,vel_y), this->body->GetWorldCenter(), true);
}

void Worm::bounce(){
  float v = Constants::worm_walk_velocity;
  if(this->direction == Right){
    v = v*(-1);
  }
  this->move(3*v, 0);
}


void Worm::move_right() {
  if(this-> direction != Right){
      this->direction = Right;
      this->calm();
      return;
  }
  this->change_state(Walk);
  float v = Constants::worm_walk_velocity;
  if(this->inclination == 0 || this->inclination == 180){
    this->move(v, 0);
    return;
  }
  if(this->inclination < 90){
    std::cout <<"[Worm] derecha con angulo menor a noventa = baja" << endl;
    this->state = Walk_down;
    this->move(v*cos(this->inclination*M_PI/180),0);//-sin(this->inclination*M_PI/180));
    this->handle_end_contact();
  }else{  
    std::cout <<"[Worm]derecha con angulo mayor a noventa = sube" << endl;
    this->state = Walk_up;
    this->move(-v*cos(this->inclination*M_PI/180),-v*sin(this->inclination*M_PI/180));
    this->handle_end_contact();
  }
}

void Worm::move_left() {
  if(this-> direction != Left){
      this->direction = Left;
      this->calm();
      return;
  }
  this->change_state(Walk);
  float v = Constants::worm_walk_velocity;
  if(this->inclination == 0 || this->inclination == 180){
    this->move(-v, 0);
    //printf("izquierdaaa\n");
    return;
  }
  else if(this->inclination < 90){
    std::cout <<"[Worm]izquierda con angulo menor a noventa = sube" << endl;
    this->state = Walk_up;
    this->move(-v*cos(this->inclination*M_PI/180),-v*sin(this->inclination*M_PI/180));
    this->handle_end_contact();
  }else{  
   std::cout << "[Worm]izquierda con angulo mayor a noventa = baja" << endl;
    this->state = Walk_down;
    this->move(cos(this->inclination*M_PI/180),0);
    this->handle_end_contact();
  }
}

//TODO: fix me!!
void Worm::jump(Direction dir) {
  this->change_state(Jump_state);
  int d = (dir == Left) ? 1 : -1;
  float impulse = body->GetMass() * Constants::worm_jump_velocity;
  this->body->ApplyLinearImpulse(b2Vec2(d * impulse /2,impulse), this->body->GetWorldCenter(), true);
}
//TODO: fix me!!
void Worm::jump_back() {
  this->change_state(Jump_back_state);
  int d = (this->direction == Left) ? 1 : -1;
  float impulse =  body->GetMass() * Constants::worm_jump_velocity;//this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(-d*impulse/2,impulse), this->body->GetWorldCenter(), true);
}

int Worm::get_life() {
  return this->life;
}

std::vector<b2Vec2> Worm::get_points() {
  std::vector<b2Vec2> points;
  for (int i = 0; i < 4; i++) {
    b2Vec2 p = ((b2PolygonShape*)this->body->GetFixtureList()->GetShape())->GetVertex(i);
    rotateTranslate(p, this->body->GetWorldCenter(), this->body->GetAngle());
    points.push_back(p);
  }

  return points;
}

void Worm::took_weapon(Weapon_Name weapon) {
  this->weapon = weapon;
  std::map<Weapon_Name,State>::iterator weapon_state = weapons_states.find(weapon);
  this->change_state(weapon_state->second);
}

bool Worm::can_use(Weapon_Name weapon) {
  return this->ammo.use(weapon);
}

void Worm::set_inclination(float angle, std::vector<b2Vec2> & beam_pos) {
  oLog() <<"[Worm] nueva inclinacion " <<angle << endl;
  this->inclination = angle;
  this->beam_pos = beam_pos;
}


void  Worm::handle_end_contact(){
  if(this->beam_pos.size() < 1){
    return;
  }
  b2Vec2 center = this->get_center();
  float left_distane= round(sqrt(pow(center.x - this->beam_pos[0].x,2) + pow(center.y - this->beam_pos[0].y,2)));
  float right_distance = round(sqrt(pow(center.x - this->beam_pos[1].x,2) + pow(center.y - this->beam_pos[1].y,2)));
  float min_dist = left_distane < right_distance ? left_distane : right_distance;
  if(min_dist > 3){
    this->no_inclination();
  }

}

void Worm::no_inclination() {
  oLog() <<"[Worm] se termino el contacto con la viga de inclinacion"<< this->inclination << endl;
  this->inclination = 0;
}


void Worm::use_weapon(float x, float y, int power, float degrees) {

}

int Worm::get_player_id() {
  return this->player_id;
}

void Worm::set_player_id(int i) {
  this->player_id = i;
}

bool Worm::disappear(){
  //printf("[Worm] disappear\n");
  if(this->teleport_counter == 0){
    this->body->SetTransform(b2Vec2(this->teleport_x,this->teleport_y),0);
    this->body->ApplyLinearImpulse(b2Vec2(0,9.8), this->body->GetWorldCenter(), true);
    this->teleport_counter = -1;
    this->state = Fall;
    return true;
  }
  this->teleport_counter--;
  return false;

}


void Worm::teleport(float x, float y) {
  this->teleport_counter = 49;
  this->teleport_x = x;
  this->teleport_y = y - Constants::worm_height/2;
  this->state = Worm_disappear;
}


void Worm::apply_damage(int d) {
  oLog() <<"[Worm] worm damaged: " << d << endl;
  this->life = this->life - d;
}

b2Vec2 Worm::get_center(){
  return this->body->GetPosition();
}

b2Vec2 Worm::get_velocity(){
  return this->body->GetLinearVelocity();
}

void Worm::stop_moving() {
  this->body->SetLinearVelocity(b2Vec2(0,0));
}

void Worm::set_static(bool first_time){
   if(this->body->GetType() ==b2_staticBody ){
    return;
  }
  b2Vec2 center = this->get_center();
  this->body->SetType(b2_staticBody);
  if(first_time){
    this->body->SetTransform(b2Vec2(center.x,center.y - Constants::worm_height/2),0); 
  }
  //printf("[wORM] STATICO\n");

}

void Worm::set_dynamic() {
  this->body->SetType(b2_dynamicBody);
}
