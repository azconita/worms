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
  Beam(b2World* world, float size, float x, float y, float angle);
  Beam(const Beam &other);
  virtual ~Beam();
  Beam* operator=(const Beam &other);
  std::vector<b2Vec2> get_points();
  int get_type() { return TYPE_BEAM; }
  float get_angle();
  b2Vec2 get_center();
};

#endif /* BEAM_H_ */
