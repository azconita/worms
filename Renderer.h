#ifndef RENDERER_H_
#define RENDERER_H_

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <thread>
#include "Logger.h"
#include "Dtos.h"
#include "BlockingQueue.h"
#include "GraphicDesigner.h"
#define INTER_TURN -2

#define SLEEPNANOSECONDS 17000000

extern  logger oLog;

class Renderer{
	int player_id;
	int worm_turn;
	SDL_Surface *screen;
	GraphicDesigner & graphic_designer;
	BlockingQueue<StageDTO> & stages_queue;
	bool on;
	std::thread principal_thread;

public:


Renderer(SDL_Surface* screen, BlockingQueue<StageDTO> & queue,GraphicDesigner & gd, int player_id);

void start();

void draw();   

int get_worm_turn();

~Renderer();
};


#endif
