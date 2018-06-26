/*
 * Entity.cpp
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#include "Entity.h"
#include <Box2D/Box2D.h>

Entity::Entity(int i) : en_type(i) {
}

Entity::~Entity() {
}

void Entity::rotateTranslate(b2Vec2& vector,const b2Vec2& center,float angle) {
  b2Vec2 tmp;
  tmp.x=vector.x*cos(angle)-vector.y*sin(angle);
  tmp.y=vector.x*sin(angle)+vector.y*cos(angle);
  vector=tmp+center;
}

