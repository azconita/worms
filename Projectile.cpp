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
std::cout << "explosionDir: " << this << '\n';
    //add box fixture
    b2CircleShape shape;
    shape.m_radius = Constants::weapon_size;
    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &shape;
    myFixtureDef.density = Constants::weapon_density;
    this->body->CreateFixture(&myFixtureDef);
    this->body->SetUserData(this);
}

Projectile::Projectile(const Projectile &other) : Entity(3), world(other.world), body(other.body), name(name) {
  std::cout << "explosionDir(&other): " << this << '\n';
  //this->body->SetUserData(this);
}

Projectile::~Projectile() {
  // TODO Auto-generated destructor stub
}

Projectile* Projectile::operator=(const Projectile &other) {
  std::cout << "explosionDir=: " << this << '\n';
  this->body = other.body;
  this->world = other.world;
  this->body->SetUserData(this);
  return this;
}

b2Vec2 Projectile::get_point() {
  return ((b2CircleShape*) this->body->GetFixtureList()->GetShape())->m_p;
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
  std::cout << "imp mag: " << impulseMag << ", blastdir: " << blastDir.x << ":" << blastDir.y << "\n";

  Entity* entity = (Entity*) (body->GetUserData());
  std::cout<<"body found: " << entity->en_type << '\n';
  if (entity->en_type == 1) {
    std::cout << "apply explosion\n";
    body->ApplyLinearImpulse( impulseMag * blastDir, body->GetPosition() , true);
  }

}


//find all bodies with fixtures in blast radius AABB
void Projectile::proximity_explosion(float radius, float power) {
  std::cout << "explosion!\n" ;
  ExplosionQueryCallback query_callback; //see "World querying topic"
  b2AABB aabb;
  b2Vec2 center = this->body->GetPosition();
  aabb.lowerBound = center - b2Vec2(radius, radius);
  aabb.upperBound = center + b2Vec2(radius, radius);
  this->world->QueryAABB(&query_callback, aabb);

  //check which of these bodies have their center of mass within the blast radius
  for (int i = 0; i < query_callback.foundBodies.size(); i++) {
      b2Body* body = query_callback.foundBodies[i];
      b2Vec2 bodyCom = body->GetWorldCenter();

      //ignore bodies outside the blast range
      if ( (bodyCom - center).Length() >= radius )
          continue;

      apply_explosion_impulse(body, center, bodyCom, power );
  }
  //this->world->DestroyBody(this->body);
  this->alive = false;
}

//r in degrees
b2Vec2 rad2vec(float r) {
  r = r * (3.14159265359/180.0);
  return b2Vec2(cos(r), sin(r));
}

void Projectile::shoot(int power, float degrees) {
  b2Vec2 vel = rad2vec(degrees);
  float velChange = power * vel.x;
  float impulsex = body->GetMass() * velChange;
  velChange = power * vel.y;
  float impulsey = body->GetMass() * velChange;
  this->body->ApplyLinearImpulse(b2Vec2(impulsex,impulsey), this->body->GetWorldCenter(), true);
}

bool Projectile::should_explode(){
  i--;
  if (this->i == 0)
    return true;
  return false;
}
