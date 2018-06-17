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

using std::string;
using std::map;
using std::pair;


#define SCREEN_DEFAULT_WIDTH 1366
#define SCREEN_DEFAULT_HIGH 768
#define TITLE "Worms game"
#define PIXEL_CONSTANT 23.5
#define TIME_TO_SLEEP 17000000

extern  logger oLog;

class Client {
	Socket socket;
	BlockingQueue<ActionDTO> actions_queue;
	SDL_Surface * screen;
	int id = -1;
public:
	Client(char * host_name, char * port);
	void run();
	StageDTO receive_stage();
	void send_action();
	float get_pixels(float meter_position);
	~Client();
};
