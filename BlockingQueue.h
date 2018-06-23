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

template <typename T>
class BlockingQueue {
private:
  std::queue<T> q;
  const unsigned int max_size;

  std::mutex mtx;
  std::condition_variable is_not_full;
  std::condition_variable is_not_empty;

public:

  BlockingQueue(const unsigned int max_size) :
                                          max_size(max_size) {
  }
  ~BlockingQueue() {
    //this->q.clear();
    printf("[BlockingQueue] deleted\n");
  }

  void push(const T& val) {
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


  T pop() {
    std::unique_lock<std::mutex> lck(this->mtx);
    while (this->q.empty()) {
      //std::cout << " cola vacia, el pull se bloquea\n";
      this->is_not_empty.wait(lck);
    }
    const T val = this->q.front();
    this->q.pop();
    this->is_not_full.notify_all();
    return val;
  }

private:
  BlockingQueue(const BlockingQueue&) = delete;
  BlockingQueue& operator=(const BlockingQueue&) = delete;

};


#endif /* BLOCKINGQUEUE_H_ */
