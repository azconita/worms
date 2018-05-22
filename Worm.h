/*
 * Worm.h
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>

#ifndef WORM_H_
#define WORM_H_

class Worm {
  b2Body* body;
  int life;
public:
  Worm(b2World* world, float x, float y);
  Worm(const Worm& other);
  Worm();
  virtual ~Worm();
  std::vector<b2Vec2> get_points();

  //movements
  void move_left(); //vel: 0.2 m/s
  void move_right();
  void jump(); // 1m adelante y 0.5m alto
  void jump_back(); // 0.2m atras y 1.2m alto

private:
  float get_impulse();
  void rotateTranslate(b2Vec2& vector,const b2Vec2& center,float angle);
};

#endif /* WORM_H_ */
