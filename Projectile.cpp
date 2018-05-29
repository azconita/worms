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
#include "DTOs.h"

Projectile::Projectile(b2World *world, Weapon_Name name, float x, float y) : Entity(3), world(world), name(name) {
  b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    bodyDef.position.Set(x, y);
    bodyDef.userData = (void*) this;
    this->body = world->CreateBody(&bodyDef);
//std::cout << "explosionDir: " << this << '\n';
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
}

Projectile::Projectile(const Projectile &other) : Entity(3), world(other.world), body(other.body), name(name) {
  //std::cout << "explosionDir(&other): " << this << '\n';
  //this->body->SetUserData(this);
}

Projectile::~Projectile() {
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
  return this->timer;
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

void apply_explosion_impulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint,
                        float blastPower) {
  b2Vec2 blastDir = applyPoint - blastCenter;
  float distance = blastDir.Normalize();
  //ignore bodies exactly at the blast point - blast direction is undefined
  if ( distance == 0 )
     return;
  float invDistance = 1 / distance;
  float impulseMag = blastPower * invDistance * invDistance;
  //std::cout << "imp mag: " << impulseMag << ", blastdir: " << blastDir.x << ":" << blastDir.y << "\n";

  Entity* entity = (Entity*) (body->GetUserData());
  std::cout<<"body found: " << entity->en_type << '\n';
  if (entity->en_type == 1) {
    std::cout << "apply explosion: "<<impulseMag <<"\n";
    body->ApplyLinearImpulse( blastPower * blastDir, body->GetPosition() , true);
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
    if (this->t == 0)
      time(&(this->t));
    else if (difftime(this->t,time(NULL)) < this->timer)
      return;
  }
  //if (this->name == Green_Grenade) &&
  std::cout << "explosion!\n" ;
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

      apply_explosion_impulse(body, center, bodyCom, power );
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

void Projectile::explosion() {
  this->timer--;
  std::cout << "timer: " << this->timer << "\n";
  if (this-> timer == 0)
    this->alive = false;
  return;
}
