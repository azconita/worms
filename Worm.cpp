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
          Entity(1), world(world), id(id) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);
  bodyDef.userData = (void*) this;
  this->body = world->CreateBody(&bodyDef);
  std::cout << "wormDir: " << this << '\n';
  //add box fixture
  b2PolygonShape shape;
  shape.SetAsBox(Constants::worm_size, Constants::worm_size);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape;
  myFixtureDef.density = Constants::worm_density;
  this->body->CreateFixture(&myFixtureDef);
  this->body->SetUserData(this);
  this->life = Constants::worm_initial_life;
  this->state = Still;
  this->direction = direction;
}

Worm::Worm(const Worm& other) : Entity(1), body(other.body), life(other.life), world(world) {
  this->body->SetUserData(this);
  std::cout << "wormDir(&other): " << this << '\n';
}

Worm::Worm() : Entity(1), body(NULL), life(0), world(NULL) {
  std::cout << "wormDir(): " << this << '\n';

}

Worm::~Worm() {
  // TODO Auto-generated destructor stub
  std::cout << "worm destroyed " << '\n';
  this->world->DestroyBody(this->body);
}

Worm* Worm::operator=(const Worm &other) {
  std::cout << "wormDir=: " << this << '\n';
  this->body = other.body;
  this->life = other.life;
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

void Worm::update_state() {
  //caer es el estado "predominante"
  b2Vec2 vel = this->body->GetLinearVelocity();
  if (vel.y > 5 && this->state != Fall) {
    this->state = Fall;
    this->start_falling = this->body->GetPosition();
    return;
  }
  if (std::abs(vel.y) < 1 && std::abs(vel.x) < 1 && this->state == Fall) {
    //estaba cayendo: chequear si fueron mas de 2m para hacerle dano al worm
    this->state = Still;
    float d = this->start_falling.y - this->body->GetPosition().y;
    if (d > 2)
      this->apply_damage((d < 25) ? d : 25);
    return;
  }
  if (this->state != Still && (this->state == Walk || this->state == Jump_state
                || this->state == Jump_back_state || this->state == Fall)
                && std::abs(vel.y) < 0.1 && std::abs(vel.x) < 0.1) {
    this->state = Still;
  }

}

Direction Worm::get_direction(){
  return this-> direction;
}

void Worm::move_right() {
  if(this-> direction != Right){
      this->direction = Right;
      change_state(Still);
      return;
  }
  this->change_state(Walk);
  float impulse = this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(Constants::worm_walk_velocity,0), this->body->GetWorldCenter(), true);
}

void Worm::move_left() {
  if(this-> direction != Left){
      this->direction = Left;
      change_state(Still);
      return;
  }
  this->change_state(Walk);

  float impulse = this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(-Constants::worm_walk_velocity,0), this->body->GetWorldCenter(), true);
}

//TODO: fix me!!
void Worm::jump(Direction dir) {
  std::cout << "dir: " << dir << "\n";
  printf("jump.....%p\n", this->body );
  this->change_state(Jump_state);
  int d = (dir == Left) ? 1 : -1;
  float impulse = body->GetMass() * Constants::worm_jump_velocity;
  this->body->ApplyLinearImpulse(b2Vec2(d * impulse /2,impulse), this->body->GetWorldCenter(), true);
}
//TODO: fix me!!
void Worm::jump_back() {
  this->change_state(Jump_back_state);
  float impulse = this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(-impulse,impulse), this->body->GetWorldCenter(), true);
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

void Worm::use_weapon(float x, float y, int power, float degrees) {

}

int Worm::get_player_id() {
  return this->player_id;
}

void Worm::teleport(float x, float y, Direction dir) {
  this->body->SetTransform(b2Vec2(x,y),body->GetAngle());
}

void Worm::apply_damage(int d) {
  std::cout << "worm damaged: " << d << "\n";
  this->life = this->life - d;
  //if (this->life <= 0)
    //this->alive = false;
}

b2Vec2 Worm::get_center(){
  return this->body->GetPosition();
}

b2Vec2 Worm::get_velocity(){
  return this->body->GetLinearVelocity();
}
