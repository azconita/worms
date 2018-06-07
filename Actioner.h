#ifndef ACTIONER_H_
#define ACTIONER_H_

#include <thread>
#include "Logger.h"
#include "Dtos.h"
#include "BlockingQueue.h"
#include "Socket.h"


class Actioner{
	Socket & socket;
	BlockingQueue<ActionDTO> & actions_queue;
	bool on;
	std::thread principal_thread;

public:
Actioner(Socket & socket, BlockingQueue<ActionDTO> & queue);
void start();
void make_action();
~Actioner();
};

#endif