#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "Error.h"
#include "Socket.h"
#include "DTOs.h"
#include "WaterAnimation.h"
#include "WormAnimation.h"
#include "EventController.h"

using std::string;
using std::map;
using std::pair;


#define SCREEN_DEFAULT_WITH 1366
#define SCREEN_DEFAULT_HIGH 768
#define TITLE "Worms game"
#define CHUNK_LEN 512


class Client{
	Socket socket;

public:
	Client(char * host_name, char * port);
	void run();
	StageDTO receive_stage();
	//para debuguear
	float get_pixels(float meter_position);
	void debug_box2d_figure(SDL_Surface *screen, ElementDTO element_info);
}