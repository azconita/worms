#include "Camera.h"

Camera::Camera(int screen_h, int screen_w){
	SDL_Surface *background = SDL_LoadBMP(BACKGROUND);
    Sint16 x = background->w/2 - screen_w/2;
    Sint16 y = background->h/2 - screen_h/2;
    Uint16 h = screen_h;
    Uint16 w = screen_w;
    this->camera_position = { x,y, w, h};

}

void Camera::move(int x, int y){
	std::unique_lock<std::mutex> lck(this->mtx);
	this->camera_position.x = this->camera_position.x + x;
	this->camera_position.y = this->camera_position.y + y;

}

SDL_Rect Camera::get_focus(){
	std::unique_lock<std::mutex> lck(this->mtx);
	return this->camera_position;

}