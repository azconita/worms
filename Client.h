#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "Error.h"
#include "Socket.h"
#include "Logger.h"
#include "Dtos.h"
#include "Actioner.h"
#include "WormAnimation.h"
#include "EventController.h"
#include "BlockingQueue.h"
#include "StageReceiver.h"

using std::string;
using std::map;
using std::pair;


#define SCREEN_DEFAULT_WIDTH 500
#define SCREEN_DEFAULT_HIGH 700
#define TITLE "Worms game"
#define PIXEL_CONSTANT 23.5
#define TIME_TO_SLEEP 1700000000
#define QUEUE_SIZE 100

extern  logger oLog;

class Client {
	Socket socket;
	BlockingQueue<ActionDTO> actions_queue;
	BlockingQueue<StageDTO> stages_queue;
	SDL_Surface * screen;
	bool fullscreen;
	int id = -1;
	bool finish;
public:
	Client(char * host_name, char * port, std::string &stage, bool fullscreen);
	void run();
	StageDTO get_stage();
	void send_action();
	float get_pixels(float meter_position);
	//void finish(int winner, GraphicDesigner & graphic_designer);
	~Client();
};
