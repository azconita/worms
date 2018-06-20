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

using std::endl;

extern logger oLog;

class Worm : public Entity {
  int id = -1;
  int player_id = 0;
  State state = Still;
  Direction direction;
  float inclination;
  b2World* world;
  b2Body* body;
  int teleport_counter = -1;
  int teleport_x;
  int teleport_y;
  Weapon_Name weapon = None;
  std::vector<b2Vec2> beam_pos;
  b2Vec2 start_falling = b2Vec2(0,0);
  int life;
public:
  Worm(b2World* world, float x, float y, int id, Direction direction);
  Worm(const    Worm& other);
  Worm();
  virtual ~Worm();
  Worm* operator=(const Worm &other);

  int get_type() { return TYPE_WORM; }
  std::vector<b2Vec2> get_points();
  b2Vec2 get_center();
  b2Vec2 get_velocity();
  int get_life();
  int get_id() { return this->id;}
  bool is_alive() { return ((this->life > 0) && (this->body->GetPosition().y < 100)); }
  int get_player_id();
  void set_player_id(int i);
  void handle_end_contact();

  //movements
  State get_state();
  Direction get_direction();
  void change_state(State state);
  void update_state();
  void move_left(); //vel: 0.2 m/s
  void move_right();
  void move(float vel_x, float vel_y);
  void jump(Direction dir); // 1m adelante y 0.5m alto
  void jump_back(); // 0.2m atras y 1.2m alto
  void stop_moving();
  void set_static(bool first_time);
  void set_dynamic();
  void set_inclination(float angle, std::vector<b2Vec2> & beam_pos);
  void no_inclination();
  void calm();

  //use weapons
  void took_weapon(Weapon_Name weapon);
  void use_weapon(float x, float y, int power, float degrees);
  void apply_damage(int d);
  void teleport(float x, float y);
  bool disappear();


  void printPos() {
    std::cout << "worm: " << this->body->GetPosition().x << ":" << this->body->GetPosition().y << "\n";
  }
  bool is_falling() { return (this->body->GetLinearVelocity().y > 0);} //no puede ser que este bajando de un salto?
private:
  float get_impulse();
  //void rotateTranslate(b2Vec2& vector,const b2Vec2& center,float angle);
};

#endif /* WORM_H_ */
