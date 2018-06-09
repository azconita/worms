
#ifndef CAMERA_H_
#define CAMERA_H_

#include <mutex>
#include <SDL/SDL.h>
#include "ResourcesDefinitions.h"

class Camera {
private:
	SDL_Rect camera_position;
	std::mutex mtx;

public:
	Camera(int screen_h, int screen_w);
	void move(int x, int y);
	SDL_Rect get_focus();
};
#endif