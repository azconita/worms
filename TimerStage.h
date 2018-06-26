/*
 * TimerStage.h
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#ifndef TIMERSTAGE_H_
#define TIMERSTAGE_H_

#include "BlockingQueue.h"
#include "CommonThread.h"
#include "Dtos.h"
#include "Logger.h"

extern  logger oLog;

class TimerStage : public Thread {
private:
  BlockingQueue<ActionDTO> &stage_queue;
  bool finished = false;
public:
  TimerStage(BlockingQueue<ActionDTO> &q);
  virtual ~TimerStage();
  virtual void run() override;
  void stop();
};

#endif /* TIMERSTAGE_H_ */
