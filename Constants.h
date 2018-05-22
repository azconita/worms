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
/*class Constants {
public:
  float gravity;
  float time_step;
  Constants(std::string configfile);
  virtual ~Constants();
};*/

} /* namespace Constants */

#endif /* CONSTANTS_H_ */
