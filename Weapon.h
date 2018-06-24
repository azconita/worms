/*
 * Explosion.h
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#ifndef WEAPON_H_
#define WEAPON_H_

#include "Entity.h"
#include <vector>
#include <iostream>
#include <Box2D/Box2D.h>
#include "Dtos.h"
#include <time.h>
#define _USE_MATH_DEFINES

#define PROJECTILE 3

class Weapon : public Entity {
private:
  b2World *world;
  float wind;
  b2Body* body;
  Weapon_Name name;
  bool alive = true;


  int counter = 0;
  int timer = 0;
  time_t t = 0;
  float radius = 0;
  float power = 0;
  float shoot_power = 0;
  float damage = 50;

  std::vector<Weapon*> *explosions;
public:
  Weapon(b2World *world, Weapon_Name name, float x, float y, float wind, std::vector<Weapon*> *explosions);
  Weapon(const Weapon &other);
  virtual ~Weapon();
  Weapon* operator=(const Weapon &other);
  int get_type() { return PROJECTILE; }
  Weapon_Name get_name() { return this->name;}
  int get_timer();
  void explosion();
  bool is_time_to_explode();
  bool has_timer();

  void raycast_explosion();
  void make_explosion(float power);
  void proximity_explosion();
  void explode();
  void apply_explosion_impulse(b2Body* body, b2Vec2 blast_direction, float distance);
  void shoot(int power, float degrees, Direction dir, int time_to_explode);

  bool is_alive();
  b2Vec2 get_center();
  std::vector<b2Vec2> get_points();

  void bazooka(int power, float degrees, int s);
  void grenade(int power, float degrees, int timer, int s);
  void dynamite(int time_to_explode, int s);
  b2Vec2 get_velocity();
  float get_angle_velocity();


  void printPos() {
      std::cout << "explosion: " << this->body->GetPosition().x << ":" << this->body->GetPosition().y << "\n";
    }
};

//subclass b2QueryCallback
class ExplosionQueryCallback : public b2QueryCallback {
public:
  std::vector<b2Body*> found_bodies;

  bool ReportFixture(b2Fixture* fixture) {
    found_bodies.push_back( fixture->GetBody() );
    return true;
  }
};

class RayCastCallback : public b2RayCastCallback {
public:
  b2Body* body;
  b2Vec2 point;

  RayCastCallback() {body = NULL;}

  float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    this->body = fixture->GetBody();
    this->point = point;
    Entity* entity = (Entity*) (this->body->GetUserData());
    std::cout<<"[raycast] found: " << entity->en_type << '\n';
    if (entity->en_type == 1)
      return fraction;
    return (float32) 1;
  }

};

#endif /* WEAPON_H_ */
