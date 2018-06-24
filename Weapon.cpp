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
    //b2CircleShape shape;
    //shape.m_radius = Constants::weapon_size;
    b2PolygonShape shape;
    shape.SetAsBox(1, 1);
    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &shape;
    myFixtureDef.density = Constants::weapon_density;
    this->body->CreateFixture(&myFixtureDef);
    this->body->SetUserData(this);
    //set weapon variables: radius and damage
    switch (name) {
      case W_Bazooka: {
        this->body->SetLinearVelocity(b2Vec2(wind, 0));
        this->body->SetLinearDamping(wind);
        this->radius = Constants::bazooka_radius;
        this->damage = Constants::bazooka_damage;
        break;
      }
      case W_Air_Attack: {
        this->body->SetLinearVelocity(b2Vec2(wind, 0));
        this->body->SetLinearDamping(wind);
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
        this->body->SetLinearVelocity(b2Vec2(wind, 0));
        this->body->SetLinearDamping(wind);
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

void Weapon::apply_explosion_impulse(b2Body* other_body, b2Vec2 blast_direction, float distance) {
  //b2Vec2 blast_dir = apply_point - blast_center;
  //float distance = blast_dir.Length();
  float inv_distance = (distance < 1) ? 1 : (1 / distance);
  float impulse_mag = this->power * inv_distance ;
  //std::cout << "imp mag: " << impulse_mag << ", blastdir: " << blast_dir.x << ":" << blast_dir.y << "\n";

  Entity* entity = (Entity*) (other_body->GetUserData());
  std::cout<<"body found: " << entity->en_type << '\n';
  if (entity->en_type == 1) {
    //std::cout << "apply explosion: "<<impulse_mag <<"\n";
    other_body->ApplyLinearImpulse( (impulse_mag/ distance) * blast_direction, other_body->GetPosition() , true);
    ((Worm*) entity)->apply_damage(int(this->damage * inv_distance));
  }

}


//find all bodies with fixtures in blast radius AABB
void Weapon::make_explosion(float power) {
  if (this->name == Explosion)
    return this->explosion();
  if (!this->alive)
    return;
  this->power = power;
  if (this->timer != 0) {
    return;
  }
  //this->radius = radius;
  this->explode();
}

bool Weapon::is_alive(){
   return (this->alive && (this->body->GetPosition().y < 60));
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

  //this->proximity_explosion();
  this->raycast_explosion();

  //this->alive = false;
  this->timer = 8;
  this->name = Explosion;
}

void Weapon::proximity_explosion() {
  ExplosionQueryCallback query_callback;
  b2AABB aabb;
  b2Vec2 center = this->body->GetPosition();
  aabb.lowerBound = center - b2Vec2(radius, radius);
  aabb.upperBound = center + b2Vec2(radius, radius);
  this->world->QueryAABB(&query_callback, aabb);

  for (int i = 0; i < query_callback.found_bodies.size(); i++) {
      b2Body* other_body = query_callback.found_bodies[i];
      b2Vec2 body_pos = other_body->GetPosition();

      //ignore bodies outside the radius
      float distance = (body_pos - center).Length();
      if (distance >= radius)
          continue;

      this->apply_explosion_impulse(other_body, body_pos - center, distance);
  }

}

void Weapon::raycast_explosion() {
  int rays = 40;
  b2Vec2 center = this->body->GetPosition();
  float dtor = 0.0174532925199432957f;
  for (int i = 0; i < rays; i++) {
    float angle = (i / (float)rays) * 360 * dtor;
    b2Vec2 ray_dir( sinf(angle), cosf(angle) );
    b2Vec2 ray_end = center + this->radius * ray_dir;

    RayCastCallback callback;//basic callback to record body and hit point
    this->world->RayCast(&callback, center, ray_end);
    if ( callback.body )
      apply_explosion_impulse(callback.body, callback.point - center, (callback.point - center).Length() - radius);
  }
}



void Weapon::shoot(int power, float degrees, Direction dir, int time_to_explode) {
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
  float vel_change = power * cos(degrees* (M_PI / 180));
  float impulsex = body->GetMass() * vel_change;
  vel_change =  power * sin(degrees*(M_PI / 180));
  float impulsey = -1*body->GetMass() * vel_change;
  printf("[Weapon] vector de velocidad = %f, %f\n",impulsex*s,impulsey );
  this->body->ApplyForce(b2Vec2(impulsex*s,impulsey), this->body->GetWorldCenter(), true);

}

void Weapon::grenade(int power, float degrees, int timer, int s) {
  this->timer = timer;
  this->t = time(NULL);
  printf("new timer: %i\n", timer);
  float vel_change = power * cos(degrees* (M_PI / 180));
  float impulsex = body->GetMass() * vel_change;
  vel_change = power * sin(degrees*(M_PI / 180));
  float impulsey = -1*body->GetMass() * vel_change;
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

float Weapon::get_angle_velocity(){
  b2Vec2 up_vec = b2Vec2(0,-1);
  b2Vec2 vel = this->body->GetLinearVelocity();

  int dot = up_vec.x*vel.x + up_vec.y*vel.y;      // dot product between [x1, y1] and [x2, y2]
  int det = up_vec.x*vel.y - up_vec.y*vel.x ;     // determinant
  float angle = atan2(det, dot)*( 180/ M_PI); // atan2(y, x) or atan2(sin, cos)

  if(angle < 0){
    angle = 360 -(-angle);
  }

  return angle;

}


bool Weapon::has_timer() {
  return (this->timer != 0);
}
