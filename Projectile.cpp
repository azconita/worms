/*
 * Explosion.cpp
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#include "Projectile.h"

#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include <string>
#include "Constants.h"
#include "Worm.h"
#include "DTOs.h"

Projectile::Projectile(b2World *world, Weapon_Name name, float x, float y, float wind) : Entity(3), world(world), wind(wind), name(name) {
  b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
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
        this->body->SetLinearVelocity(b2Vec2(0,wind));
        this->radius = Constants::bazooka_radius;
        this->damage = Constants::bazooka_damage;
        break;
      }
      case W_Air_Attack: {
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
        this->radius = Constants::mortar_radius;
        this->damage = Constants::mortar_damage;
        break;
      }
      case Green_Granade: {
        this->radius = Constants::greengrenade_radius;
        this->damage = Constants::greengrenade_damage;
        break;
      }
      case Holy_Granade: {
        this->radius = Constants::holygrenade_radius;
        this->damage = Constants::holygrenade_damage;
        break;
      }
      case Red_Granade: {
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
        this->radius = Constants::banana_radius;
        this->damage = Constants::banana_damage;
        break;
      }
      case Baseboll_Bat: {
        this->radius = Constants::baseballbat_radius;
        this->damage = Constants::baseballbat_damage;
        break;
      }
    }
std::cout << "explosionDir: " << this << '\n';
}

Projectile::Projectile(const Projectile &other) : Entity(3), world(other.world), wind(other.wind), body(other.body), name(name) {
  //std::cout << "explosionDir(&other): " << this << '\n';
  //this->body->SetUserData(this);
}

Projectile::~Projectile() {
  std::cout << "weapon destroyed: " << this->name << '\n';
  this->world->DestroyBody(this->body);
}

Projectile* Projectile::operator=(const Projectile &other) {
  //std::cout << "explosionDir=: " << this << '\n';
  this->body = other.body;
  this->world = other.world;
  this->body->SetUserData(this);
  return this;
}

int Projectile::get_timer() {
  //return this->timer;
  return difftime(time(NULL), this->t);
}

b2Vec2 Projectile::get_point() {
  //return ((b2CircleShape*) this->body->GetFixtureList()->GetShape())->m_p;
  return this->body->GetPosition();
}

std::vector<b2Vec2> Projectile::get_points() {
  std::vector<b2Vec2> points;
  for (int i = 0; i < 4; i++) {
    b2Vec2 p = ((b2PolygonShape*)this->body->GetFixtureList()->GetShape())->GetVertex(i);
    rotateTranslate(p, this->body->GetWorldCenter(), this->body->GetAngle());
    points.push_back(p);
  }

  return points;
}

void Projectile::apply_explosion_impulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint,
                        float blastPower) {
  b2Vec2 blast_dir = applyPoint - blastCenter;
  float distance = blast_dir.Normalize();
  //ignore bodies exactly at the blast point - blast direction is undefined
  if ( distance == 0 )
     return;
  float invDistance = 1 / distance;
  float impulse_mag = blastPower * invDistance * invDistance;
  //std::cout << "imp mag: " << impulseMag << ", blastdir: " << blastDir.x << ":" << blastDir.y << "\n";

  Entity* entity = (Entity*) (body->GetUserData());
  std::cout<<"body found: " << entity->en_type << '\n';
  if (entity->en_type == 1) {
    std::cout << "apply explosion: "<<impulse_mag <<"\n";
    //((Worm*) entity)->apply_explosion_impulse(blast_dir * impulse_mag, float distance);

    body->ApplyLinearImpulse( impulse_mag * blast_dir, body->GetPosition() , true);
    ((Worm*) entity)->apply_damage(int(this->damage * invDistance));
  }

}


//find all bodies with fixtures in blast radius AABB
void Projectile::proximity_explosion(float radius, float power) {
  if (this->name == Explosion)
    return this->explosion();
  if (!this->alive)
    return;
  //TODO: timer!!
  if (this->timer != 0) {
    this->name = W_Timer;
    std::cout << "timer: " << this->timer << "\n";
      time(&(this->t));
    std::cout << "t: " << this->t << "\n";
    return;
    if (this->t == 0) {
      return;
    }
    if (difftime(this->t,time(NULL)) < this->timer)
      return;
    return;
  }
  //if (this->name == Green_Grenade) &&
  this->radius = radius;
  this->power = power;
  this->explode();
}

void Projectile::explode() {
  std::cout << "explosion!\n" ;
  if (this->name == Red_Granade) {
    int d = 15;
    for (int i = 0; i < 6; ++i) {
      Projectile* w = new Projectile(this->world, W_Fragment, this->body->GetPosition().x, this->body->GetPosition().y, this->wind);
      w->shoot(this->power, d, Right, 0);
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
      b2Body* body = query_callback.foundBodies[i];
      b2Vec2 bodyCom = body->GetWorldCenter();

      //ignore bodies outside the blast range
      if ( (bodyCom - center).Length() >= radius )
          continue;

      this->apply_explosion_impulse(body, center, bodyCom, power );
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

void Projectile::shoot(int power, float degrees, Direction dir, int time_to_explode) {
  printf("[Projectile] shooting %d", this->name);
  int s = (dir == Right) ? 1 : -1;
  switch (this->name) {
    case W_Bazooka: {
      this->bazooka(power,degrees, s);
      break;
    }
    case Green_Granade: {
      this->green_grenade(power, degrees, time_to_explode, s);
      break;
    }
    case Dynamite: {
      this->dynamite(time_to_explode, s);
      break;
    }
    case W_Air_Attack: {
      //this->airattack();
      break;
    }
  }
}

void Projectile::bazooka(int power, float degrees, int s) {
  //TODO:afectar por el viento
  b2Vec2 vel = rad2vec(degrees);
  float velChange = power * vel.x;
  float impulsex = body->GetMass() * velChange;
  velChange = power * vel.y;
  float impulsey = body->GetMass() * velChange;
  this->body->ApplyLinearImpulse(b2Vec2(impulsex*s,impulsey), this->body->GetWorldCenter(), true);

}

void Projectile::green_grenade(int power, float degrees, int timer, int s) {
  this->timer = timer;
  b2Vec2 vel = rad2vec(degrees);
  float velChange = power * vel.x;
  float impulsex = body->GetMass() * velChange;
  velChange = power * vel.y;
  float impulsey = body->GetMass() * velChange;
  this->body->ApplyLinearImpulse(b2Vec2(impulsex*s,impulsey), this->body->GetWorldCenter(), true);
}

void Projectile::dynamite(int time_to_explode, int s) {
  this->timer = timer;

}

//void Projectile::airattack() {
  //this->body->SetTransform(b2Vec2(this->x, this->y));
//}

void Projectile::explosion() {
  this->timer--;
  if (this-> timer == 0)
    this->alive = false;
  return;
}

bool Projectile::is_time_to_explode() {
  if ((this->timer != 0) && (this->name == W_Timer)) {
    std::cout << "difftime: " << difftime(time(NULL),this->t) << "\n";
    std::cout << "t: " << this->t << "\n";
    if (difftime(time(NULL), this->t) < this->timer)
      return false;
    return true;
  }
  return false;
}

bool Projectile::has_timer() {
  return (this->timer != 0);
}
