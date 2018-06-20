/*
 * Beam.h
 *
 *  Created on: May 20, 2018
 *      Author: gabi
 */
#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include <vector>
#include <math.h>

#include "Entity.h"
#include "Constants.h"
#include "Logger.h"
#include "Dtos.h"

#ifndef BEAM_H_
#define BEAM_H_

#define TYPE_BEAM 1

class Beam : public Entity {
private:
  b2Body* body;
  Direction direction;
  float angle;
public:
  Beam(b2World* world, float size, float x, float y, float angle, Direction direction);
  Beam(const Beam &other);
  b2Vec2 get_velocity();
  virtual ~Beam();
  Beam* operator=(const Beam &other);
  std::vector<b2Vec2> get_points();
  int get_type() { return TYPE_BEAM; }
  float get_angle();
  b2Vec2 get_center();
  Direction get_direction();
};

#endif /* BEAM_H_ */
