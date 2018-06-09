
#ifndef CAMERA_H_
#define CAMERA_H_

#include <mutex>
#include <SDL/SDL.h>
#include "ResourcesDefinitions.h"

class Camera {
private:
	SDL_Rect camera_pos;
	int max_height, max_width;
	std::mutex mtx;

	void keep_camera_pos_in_bounds();

public:
	Camera(int screen_h, int screen_w);
	void move(int x, int y);
	SDL_Rect get_focus();
};
#endif