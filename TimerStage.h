/*
 * TimerStage.h
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#ifndef TIMERSTAGE_H_
#define TIMERSTAGE_H_

#include "common_thread.h"
#include "DTOs.h"
#include "BlockingQueue.h"

class TimerStage : public Thread {
private:
  BlockingQueue<ActionDTO> &stage_queue;
  bool finished = false;
public:
  TimerStage(BlockingQueue<ActionDTO> &q);
  virtual ~TimerStage();
  virtual void run() override;
};

#endif /* TIMERSTAGE_H_ */
