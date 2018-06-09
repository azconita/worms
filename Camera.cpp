#include "Camera.h"

Camera::Camera(int screen_h, int screen_w){
	SDL_Surface *background = SDL_LoadBMP(BACKGROUND);
	this->max_width = background->w;
	this->max_height = background->h;
    Sint16 x = background->w/2 - screen_w/2;
    Sint16 y = background->h/2 - screen_h/2;
    Uint16 h = screen_h;
    Uint16 w = screen_w;
    this->camera_pos = { x,y, w, h};

}

void Camera::move(int x, int y){
	std::unique_lock<std::mutex> lck(this->mtx);
	this->camera_pos.x = this->camera_pos.x + x;
	this->camera_pos.y = this->camera_pos.y + y;
	this->keep_camera_pos_in_bounds();
	    


}

void Camera::keep_camera_pos_in_bounds(){
		if( this->camera_pos.x < 0 ){
	        this->camera_pos.x = 0;    
	    }
	    if( this->camera_pos.y < 0 ){
	        this->camera_pos.y = 0;    
	    }

	    printf("x = %i max = %i\n", this->camera_pos.x + this->camera_pos.w, this->max_width );
	    
	    if( this->camera_pos.x + this->camera_pos.w > this->max_width){
	        this->camera_pos.x = this->max_width - this->camera_pos.w;   
	        printf("quedaaaaa %i\n", this->camera_pos.x ); 
	    }
	    if( this->camera_pos.y + this->camera_pos.h> this->max_height){
	        this->camera_pos.y = this->max_height - this->camera_pos.h;    
	    }
}


SDL_Rect Camera::get_focus(){
	std::unique_lock<std::mutex> lck(this->mtx);
	return this->camera_pos;

}


