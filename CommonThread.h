#ifndef THREAD_H_
#define THREAD_H_

#include <thread>

/*
 * Interfaz usada por: Server, Game, TimerStage
 * Estas clases implementan el metodo run()
 */
class Thread {
 private:
  std::thread thread;

 public:
  Thread() {}
  virtual ~Thread() {}
  void start();
  void join();
  virtual void run() = 0;
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;
  Thread(Thread&& other);
  Thread& operator=(Thread&& other);
};

#endif
