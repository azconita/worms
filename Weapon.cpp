/*
 * Explosion.cpp
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#include "Weapon.h"

#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include <string>
#include "Constants.h"
#include "Worm.h"
#include "Dtos.h"

Weapon::Weapon(b2World *world, Weapon_Name name, float x, float y, float wind, std::vector<Weapon*> *explosions) :
                Entity(3), world(world), wind(wind), name(name), explosions(explosions) {
  b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    //fix position of shooting: explota contra el gusano que dispara
    bodyDef.position.Set(x, y);
    bodyDef.userData = (void*) this;
    this->body = world->CreateBody(&bodyDef);
    //add box fixture
    b2CircleShape shape;
    shape.m_radius = Constants::weapon_size;
    //b2PolygonShape shape;
    //shape.SetAsBox(0.5, 0.5);
    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &shape;
    myFixtureDef.density = Constants::weapon_density;
    this->body->CreateFixture(&myFixtureDef);
    this->body->SetUserData(this);
    //set weapon variables: radius and damage
    switch (name) {
      case W_Bazooka: {
        //this->body->SetLinearVelocity(b2Vec2(0,wind));
        this->body->SetLinearDamping(wind);
        this->radius = Constants::bazooka_radius;
        this->damage = Constants::bazooka_damage;
        break;
      }
      case W_Air_Attack: {
        this->body->SetLinearVelocity(b2Vec2(0,wind));
        this->radius = Constants::airattack_radius;
        this->damage = Constants::airattack_damage;
        break;
      }
      case Dynamite: {
        this->radius = Constants::dynamite_radius;
        this->damage = Constants::dynamite_damage;
        break;
      }
      case Mortar: {
        this->body->SetLinearVelocity(b2Vec2(0,wind));
        this->radius = Constants::mortar_radius;
        this->damage = Constants::mortar_damage;
        break;
      }
      case Green_Grenade: {
        this->radius = Constants::greengrenade_radius;
        this->damage = Constants::greengrenade_damage;
        break;
      }
      case Holy_Grenade: {
        this->radius = Constants::holygrenade_radius;
        this->damage = Constants::holygrenade_damage;
        break;
      }
      case Red_Grenade: {
        this->radius = Constants::redgrenade_radius;
        this->damage = Constants::redgrenade_damage;
        break;
      }
      case W_Fragment: {
        this->radius = Constants::fragment_radius;
        this->damage = Constants::fragment_damage;
        break;
      }
      case Banana: {
        this->body->SetLinearVelocity(b2Vec2(0,wind));
        this->radius = Constants::banana_radius;
        this->damage = Constants::banana_damage;
        break;
      }
      case Baseball_Bat: {
        this->radius = Constants::baseballbat_radius;
        this->damage = Constants::baseballbat_damage;
        break;
      }
    }
}

Weapon::Weapon(const Weapon &other) : Entity(3),
                                      world(other.world),
                                      wind(other.wind),
                                      body(other.body),
                                      name(other.name),
                                      explosions(other.explosions) {
}

Weapon::~Weapon() {
  std::cout << "weapon destroyed: " << this->name << '\n';
  this->world->DestroyBody(this->body);
}

Weapon* Weapon::operator=(const Weapon &other) {
  //std::cout << "explosionDir=: " << this << '\n';
  this->body = other.body;
  this->world = other.world;
  this->body->SetUserData(this);
  return this;
}

int Weapon::get_timer() {
  //return this->timer;
  return difftime(time(NULL), this->t);
}

b2Vec2 Weapon::get_center() {
  return this->body->GetPosition();
}

std::vector<b2Vec2> Weapon::get_points() {
  std::vector<b2Vec2> points;
  for (int i = 0; i < 4; i++) {
    b2Vec2 p = ((b2PolygonShape*)this->body->GetFixtureList()->GetShape())->GetVertex(i);
    rotateTranslate(p, this->body->GetWorldCenter(), this->body->GetAngle());
    points.push_back(p);
  }

  return points;
}

void Weapon::apply_explosion_impulse(b2Body* other_body, b2Vec2 blast_center, b2Vec2 apply_point,
                        float blast_power) {
  b2Vec2 blast_dir = apply_point - blast_center;
  float distance = blast_dir.Normalize();
  float inv_distance = (distance < 1) ? 1 : (1 / distance);
  float impulse_mag = blast_power * inv_distance ;
  //std::cout << "imp mag: " << impulse_mag << ", blastdir: " << blast_dir.x << ":" << blast_dir.y << "\n";

  Entity* entity = (Entity*) (other_body->GetUserData());
  //std::cout<<"body found: " << entity->en_type << '\n';
  if (entity->en_type == 1) {
    //std::cout << "apply explosion: "<<impulse_mag <<"\n";
    other_body->ApplyLinearImpulse( impulse_mag * blast_dir, other_body->GetPosition() , true);
    ((Worm*) entity)->apply_damage(int(this->damage * inv_distance));
  }

}


//find all bodies with fixtures in blast radius AABB
void Weapon::proximity_explosion(float power) {
  if (this->name == Explosion)
    return this->explosion();
  if (!this->alive)
    return;
  this->power = power;
  //TODO: timer!!
  if (this->timer != 0) {
    //this->name = W_Timer;
    //std::cout << "timer: " << this->timer << "\n";
    //time(&(this->t));
    //std::cout << "t: " << this->t << "\n";
    return;
    /*if (this->t == 0) {
      return;
    }
    if (difftime(this->t,time(NULL)) < this->timer)
      return;
    return;*/
  }
  //if (this->name == Green_Grenade) &&
  //this->radius = radius;
  this->explode();
}

bool Weapon::is_alive(){
   return (this->alive && (this->body->GetPosition().y < 100)); 
}

void Weapon::explode() {
  std::cout << "explosion!\n" ;
  if (this->name == Red_Grenade || this->name == Mortar) {
    std::cout << "more explosions!!\n";
    int d = 15;
    for (int i = 0; i < 6; ++i) {
      Weapon* w = new Weapon(this->world, W_Fragment, this->body->GetPosition().x, this->body->GetPosition().y, this->wind, this->explosions);
      this->explosions->push_back(w);
      w->shoot(this->shoot_power, d, Right, i * 30);
      d = d + 30;
    }
  }
  ExplosionQueryCallback query_callback;
  b2AABB aabb;
  b2Vec2 center = this->body->GetPosition();
  aabb.lowerBound = center - b2Vec2(radius, radius);
  aabb.upperBound = center + b2Vec2(radius, radius);
  this->world->QueryAABB(&query_callback, aabb);

  for (int i = 0; i < query_callback.foundBodies.size(); i++) {
      b2Body* other_body = query_callback.foundBodies[i];
      b2Vec2 bodyCom = other_body->GetPosition();

      //ignore bodies outside the blast range
      if ( (bodyCom - center).Length() >= radius )
          continue;

      this->apply_explosion_impulse(other_body, center, bodyCom, power );
  }
  //this->alive = false;

  this->timer = 8;
  this->name = Explosion;
}

//r in degrees
b2Vec2 rad2vec(float r) {
  r = r * (3.14159265359/180.0);
  return b2Vec2(cos(r), -sin(r));
}

void Weapon::shoot(int power, float degrees, Direction dir, int time_to_explode) {
  printf("[Weapon] shooting %d", this->name);
  this->shoot_power = power;
  int s = (dir == Right) ? 1 : -1;
  switch (this->name) {
    case W_Bazooka: {
      this->bazooka(power,degrees, s);
      break;
    }
    case Green_Grenade: {
      this->grenade(power, degrees, time_to_explode, s);
      break;
    }
    case Red_Grenade: {
      this->grenade(power, degrees, time_to_explode, s);
      break;
    }
    case Holy_Grenade: {
      this->grenade(power, degrees, time_to_explode, s);
      break;
    }
    case Dynamite: {
      this->dynamite(time_to_explode, s);
      break;
    }
    case Mortar: {
      this->bazooka(power, degrees, s);
      break;
    }
    case W_Fragment: {
      this->bazooka(power, degrees, s);
      break;
    }
  }
}

void Weapon::bazooka(int power, float degrees, int s) {
  //TODO:afectar por el viento
  b2Vec2 vel = rad2vec(degrees);
  float vel_change = power * vel.x;
  float impulsex = body->GetMass() * vel_change;
  vel_change = power * vel.y;
  float impulsey = body->GetMass() * vel_change;
  this->body->ApplyLinearImpulse(b2Vec2(impulsex*s,impulsey), this->body->GetWorldCenter(), true);

}

void Weapon::grenade(int power, float degrees, int timer, int s) {
  this->timer = timer;
  this->t = time(NULL);
  printf("new timer: %i\n", timer);
  b2Vec2 vel = rad2vec(degrees);
  float velChange = power * vel.x;
  float impulsex = body->GetMass() * velChange;
  velChange = power * vel.y;
  float impulsey = body->GetMass() * velChange;
  this->body->ApplyLinearImpulse(b2Vec2(impulsex*s,impulsey), this->body->GetWorldCenter(), true);
}

void Weapon::dynamite(int time_to_explode, int s) {
  this->timer = time_to_explode;
  this->t = time(NULL);
}

void Weapon::explosion() {
  this->timer--;
  if (this-> timer == 0)
    this->alive = false; 
    printf("[Weapon] exploto, ya se podria borrar\n");
  return;
}

bool Weapon::is_time_to_explode() {
  if ((this->timer != 0) && (this->name == Green_Grenade || this->name == Red_Grenade
                          || this->name == Dynamite || this->name == Holy_Grenade)) {
    //std::cout << "t: " << difftime(time(NULL), this->t) << "\n";
    if (difftime(time(NULL), this->t) <= this->timer)
      return false;
    return true;
  }
  return false;
}


b2Vec2 Weapon::get_velocity(){
  return this->body->GetLinearVelocity();
}


bool Weapon::has_timer() {
  return (this->timer != 0);
}
