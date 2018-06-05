/*
 * TimerStage.h
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#ifndef TIMERSTAGE_H_
#define TIMERSTAGE_H_

#include "common_thread.h"

class TimerStage : public Thread {
public:
  TimerStage();
  virtual ~TimerStage();

  virtual void run() override;
};

#endif /* TIMERSTAGE_H_ */
