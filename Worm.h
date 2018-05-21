/*
 * Worm.h
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */

#include <Box2D/Box2D.h>
#include <vector>

#ifndef WORM_H_
#define WORM_H_

class Worm {
  b2Body* body;
public:
  Worm(b2World* world, float x, float y);
  virtual ~Worm();
  std::vector<b2Vec2> get_points();
};

#endif /* WORM_H_ */
