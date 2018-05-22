/*
 * Constants.h
 *
 *  Created on: May 22, 2018
 *      Author: gabi
 */

#include <string>

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace Constants {

  extern const float gravity;
  extern const float time_step;
  extern const int velocity_iterations;
  extern const int position_iterations;

  //worm constants
  extern const int worm_initial_life;
  extern const float worm_walk_velocity;
  extern const float worm_density;
  extern const float worm_size; //lado del cuadrado

} /* namespace Constants */

#endif /* CONSTANTS_H_ */
