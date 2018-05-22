/*
 * Beam.h
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */
#include <Box2D/Box2D.h>
#include <vector>

#ifndef BEAM_H_
#define BEAM_H_

class Beam {
private:
  float x;
  float y;
  b2Body* body;
public:
  Beam(b2World* world, float x, float y);
  virtual ~Beam();
  std::vector<b2Vec2> get_points();
};

#endif /* BEAM_H_ */	