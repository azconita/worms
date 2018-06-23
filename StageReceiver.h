#ifndef STAGERECEIVER_H_
#define STAGERECEIVER_H_

#include <thread>
#include "Dtos.h"
#include "BlockingQueue.h"
#include "Socket.h"


class StageReceiver{
	Socket & socket;
	BlockingQueue<StageDTO> & stages_queue;
	bool on;
	std::thread principal_thread;

public:
StageReceiver(Socket & socket, BlockingQueue<StageDTO> & queue);
void start();
void receive_stage();
~StageReceiver();
};

#endif