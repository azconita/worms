/*
 * Entity.h
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <Box2D/Box2D.h>

class Entity {
public:
  int en_type;
public:
  //Entity() : en_type(-1) {}
  Entity(int i);
  virtual ~Entity();
  virtual int get_type()=0;
  void rotateTranslate(b2Vec2& vector,const b2Vec2& center,float angle);
};

#endif /* ENTITY_H_ */
