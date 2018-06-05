/*
 * Worm.h
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include "Weapon.h"
#include "Entity.h"
#include "Dtos.h"

#ifndef WORM_H_
#define WORM_H_

#define TYPE_WORM 1

class Worm : public Entity {
  int id = -1;
  int player_id = 0;
  b2World* world;
  b2Body* body;
  Weapon_Name weapon = None;
  int life;
public:
  Worm(b2World* world, float x, float y, int id);
  Worm(const    Worm& other);
  Worm();
  virtual ~Worm();
  Worm* operator=(const Worm &other);

  int get_type() { return TYPE_WORM; }
  std::vector<b2Vec2> get_points();
  b2Vec2 get_position() { return this->body->GetPosition();}
  int get_life();
  int get_id() { return this->id;}
  int get_player_id() { return this->player_id;}
  bool is_alive() { return ((this->life > 0) && (this->body->GetPosition().y < 100)); }

  //movements
  void move_left(); //vel: 0.2 m/s
  void move_right();
  void jump(Direction dir); // 1m adelante y 0.5m alto
  void jump_back(); // 0.2m atras y 1.2m alto

  //use weapons
  void took_weapon(Weapon_Name weapon);
  void use_weapon(float x, float y, int power, float degrees);
  void apply_damage(int d);
  void teleport(float x, float y, Direction dir);

  void printPos() {
    std::cout << "worm: " << this->body->GetPosition().x << ":" << this->body->GetPosition().y << "\n";
  }
  bool is_falling() { return (this->body->GetLinearVelocity().y > 0);}
private:
  float get_impulse();
  //void rotateTranslate(b2Vec2& vector,const b2Vec2& center,float angle);
};

#endif /* WORM_H_ */
