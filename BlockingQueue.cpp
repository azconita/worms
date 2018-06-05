/*
 * BlockingQueue.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "BlockingQueue.h"

template<class T>
BlockingQueue<T>::BlockingQueue(const unsigned int max_size) :
                                          max_size(max_size) {
}

template<class T>
BlockingQueue<T>::~BlockingQueue() {
}

template<class T>
void BlockingQueue<T>::push(const T& val) {
  std::unique_lock<std::mutex> lck(this->mtx);
  if (this->q.empty()) {
    this->is_not_empty.notify_all();
  }

  while (this->q.size() >= this->max_size) {
    //queue full: push blocked
    is_not_full.wait(lck);
  }
  q.push(val);
}

template<class T>
T BlockingQueue<T>::pop() {
  std::unique_lock<std::mutex> lck(this->mtx);
  while (this->q.empty()) {
    //std::cout << " cola vacia, el pull se bloquea\n";
    this->is_not_empty.wait(lck);
  }
  const int val = this->q.front();
  this->q.pop();
  this->is_not_full.notify_all();
  return val;
}
