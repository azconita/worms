/*
 * TimerStage.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "TimerStage.h"
#include <chrono>
#include <thread>

TimerStage::TimerStage(BlockingQueue<ActionDTO> &q) : stage_queue(q){
  // TODO Auto-generated constructor stub

}

TimerStage::~TimerStage() {
  // TODO Auto-generated destructor stub
}

void TimerStage::run() {
  while (!this->finished) {
    //printf("[TimerStage] push update\n");
    //TODO fix this
    std::this_thread::sleep_for(std::chrono::milliseconds(17));
    ActionDTO action;
    action.type = Timer_update;
    this->stage_queue.push(action);
  }
}

void TimerStage::stop() {
  this->finished = true;
}
