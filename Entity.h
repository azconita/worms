/*
 * Entity.h
 *
 *  Created on: May 26, 2018
 *      Author: gabi
 */

#ifndef ENTITY_H_
#define ENTITY_H_

class Entity {
public:
  int en_type;
public:
  //Entity() : en_type(-1) {}
  Entity(int i);
  virtual ~Entity();
  virtual int get_type()=0;
};

#endif /* ENTITY_H_ */
