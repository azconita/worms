/*
 * Worm.cpp
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include "Worm.h"
#include "Constants.h"
#include <string>

Worm::Worm(b2World* world, float x, float y, int id) :
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
  printf("creacion   %p\n", this->body );
  this->life = Constants::worm_initial_life;
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

void Worm::move_right() {
  float impulse = this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(Constants::worm_walk_velocity,0), this->body->GetWorldCenter(), true);
}

void Worm::move_left() {
  float impulse = this->get_impulse();
  this->body->ApplyLinearImpulse(b2Vec2(-Constants::worm_walk_velocity,0), this->body->GetWorldCenter(), true);
}

//TODO: fix me!!
void Worm::jump(Direction dir) {
  std::cout << "dir: " << dir << "\n";
  printf("jump.....%p\n", this->body );
  int d = (dir == Left) ? 1 : -1;
  float impulse = body->GetMass() * Constants::worm_jump_velocity;
  this->body->ApplyLinearImpulse(b2Vec2(d * impulse /2,impulse), this->body->GetWorldCenter(), true);
}
//TODO: fix me!!
void Worm::jump_back() {
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
