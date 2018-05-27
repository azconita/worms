/*
 * Beam.h
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */
#include <Box2D/Box2D.h>
#include <vector>

#include "Entity.h"

#ifndef BEAM_H_
#define BEAM_H_

#define TYPE_BEAM 1

class Beam : public Entity {
private:
  b2Body* body;
public:
  Beam(b2World* world, float x, float y);
  Beam(const Beam &other);
  virtual ~Beam();
  Beam* operator=(const Beam &other);
  std::vector<b2Vec2> get_points();
  int get_type() { return TYPE_BEAM; }
};

#endif /* BEAM_H_ */
