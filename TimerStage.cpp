/*
 * TimerStage.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "TimerStage.h"
#include <chrono>
#include <thread>

#define SLEEPNANOSECONDS 100000000

TimerStage::TimerStage(BlockingQueue<ActionDTO> &q) : stage_queue(q){

}

TimerStage::~TimerStage() {
  printf("[TimerStage] deleted\n");
}

void TimerStage::run() {
  using delta = std::chrono::duration<double, std::nano>;
  std::chrono::nanoseconds diff(0);
  while (!this->finished) {
    //printf("[TimerStage] push update\n");
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::nanoseconds(SLEEPNANOSECONDS) - diff);
    ActionDTO action;
    action.type = Timer_update;
    this->stage_queue.push(action);
    auto end = std::chrono::high_resolution_clock::now();

    diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start) - std::chrono::nanoseconds(SLEEPNANOSECONDS);
    diff = (diff.count() > 0) ? diff : std::chrono::nanoseconds(0);
  }
}

void TimerStage::stop() {
  this->finished = true;
}
