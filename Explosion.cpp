/*
 * Explosion.cpp
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include <string>
#include "Constants.h"

#include "Explosion.h"

Explosion::Explosion(b2World *world, float x, float y) : Entity(3), world(world) {
  // TODO Auto-generated constructor stub
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

Explosion::Explosion(const Explosion &other) : Entity(3), world(other.world), body(other.body) {
  std::cout << "explosionDir(&other): " << this << '\n';
  //this->body->SetUserData(this);
}

Explosion::~Explosion() {
  // TODO Auto-generated destructor stub
}

Explosion* Explosion::operator=(const Explosion &other) {
  std::cout << "explosionDir=: " << this << '\n';
  this->body = other.body;
  this->world = other.world;
  this->body->SetUserData(this);
  return this;
}

void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint,
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
void Explosion::proximityExplosion(float blastRadius, float blastPower) {
  std::cout << "explosion!\n" ;
  ExplosionQueryCallback queryCallback; //see "World querying topic"
  b2AABB aabb;
  b2Vec2 center = this->body->GetPosition();
  aabb.lowerBound = center - b2Vec2( blastRadius, blastRadius );
  aabb.upperBound = center + b2Vec2( blastRadius, blastRadius );
  this->world->QueryAABB( &queryCallback, aabb );

  //check which of these bodies have their center of mass within the blast radius
  for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
      b2Body* body = queryCallback.foundBodies[i];
      b2Vec2 bodyCom = body->GetWorldCenter();

      //ignore bodies outside the blast range
      if ( (bodyCom - center).Length() >= blastRadius )
          continue;

      applyBlastImpulse(body, center, bodyCom, blastPower );
  }
}

bool Explosion::should_explode(){
  i--;
  if (this->i == 0)
    return true;
  return false;
}
