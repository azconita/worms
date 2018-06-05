/*
 * BlockingQueue.h
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */



#ifndef BLOCKINGQUEUE_H_
#define BLOCKINGQUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>

template <class T>
class BlockingQueue {
private:
  std::queue<T> q;
  const unsigned int max_size;

  std::mutex mtx;
  std::condition_variable is_not_full;
  std::condition_variable is_not_empty;

public:
  BlockingQueue(const unsigned int max_size);
  virtual ~BlockingQueue();

  void push(const T& val);
  T pop();
private:
  BlockingQueue(const BlockingQueue&) = delete;
  BlockingQueue& operator=(const BlockingQueue&) = delete;

};

#endif /* BLOCKINGQUEUE_H_ */
