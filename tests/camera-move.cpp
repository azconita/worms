#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

//The dot dimensions
#define DOT_WIDTH 20
#define DOT_HEIGHT 20


//The dot
class Dot{
    private:
    //The X and Y offsets of the dot
    int x, y;


    public:
    //Initializes the variables
    Dot(int x, int y){
    	this->x = x;
    	this->y = y;

    }

    void right(){
    	this->x +=15;
    }
    
    void left(){
    	this->x -=15;
    }
    
    void up(){
    	this->y -=15;
    }
    
    void down(){
    	this->y +=15;
    }
    
    //Shows the dot on the screen
    void show(SDL_Rect & camera_pos, SDL_Surface *screen){
    	//Show the dot
    	 SDL_Rect rectangle;
    	rectangle.x = x - camera_pos.x;
    	rectangle.y = y - camera_pos.y;
    	rectangle.h = DOT_HEIGHT;
    	rectangle.w = DOT_WIDTH; 
    	Uint32 colorkey = SDL_MapRGBA(screen->format, 255, 255, 255,0.5);
    	SDL_FillRect(screen, &rectangle, colorkey);
    	
    	//apply_surface( x - camera_pos.x, y - camera_pos.y, this, screen );
    }
    
    //Sets the camera_pos over the dot
    void set_camera_pos(SDL_Rect & camera_pos, int screen_width, int screen_height){
	    //Center the camera_pos over the dot
	    camera_pos.x = ( this->x + DOT_WIDTH / 2 ) - screen_width / 2;
	    camera_pos.y = ( this->y + DOT_HEIGHT / 2 ) - screen_height / 2;    
	}

	 void follow(SDL_Rect & camera_pos, int screen_width, int screen_height){
	    //Center the camera_pos over the dot
	    if(this->x < camera_pos.x ){
	    	camera_pos.x = this->x; 
	    }
	    if(this->x > camera_pos.x + screen_height){
	    	camera_pos.x = this->x -screen_height;
	    }
	    if(this->y < camera_pos.y ){
	    	camera_pos.y = this->y;
	    } 
	    if(this->y > camera_pos.y + screen_width){
	    	camera_pos.y = this->y -screen_width;
	    }
	}
};


void keep_camera_pos_in_bounds(SDL_Rect & camera_pos, int max_height, int max_width){
	//Keep the camera_pos in bounds.
	    if( camera_pos.x < 0 ){
	        camera_pos.x = 0;    
	    }
	    if( camera_pos.y < 0 ){
	        camera_pos.y = 0;    
	    }
	    if( camera_pos.x > max_width - camera_pos.w ){
	        camera_pos.x = max_width - camera_pos.w;    
	    }
	    if( camera_pos.y > max_height - camera_pos.h ){
	        camera_pos.y = max_height - camera_pos.h;    
	    }
}




bool event_controller(SDL_Event &event,  Dot &myDot, SDL_Rect &camera_pos , int screen_height, int screen_width){
	if(SDL_PollEvent(&event) != 1){
        return true; // no hay nuevos event
    }
    switch(event.type){
        case SDL_QUIT:
            return false;
        case SDL_MOUSEMOTION:
            int x, y;
            //printf("pantalla= w:%i h:%i\n", screen_width, screen_height);
            //printf("mouse = x:%i y:%i\n", x, y);
    		SDL_GetMouseState(&x, &y);
    		if(x <=2){
    			camera_pos.x -= 2;
    		}
    		if(x >= screen_width -2 ){
    			camera_pos.x += 2;
    		}
    		if(y <= 2){
    			camera_pos.y -= 2;
    		}
    		if(y >= screen_height -2){
    			printf("deberia baajar\n");
    			camera_pos.y += 2;
    		}
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE ){
                return false;
            }
            switch(event.key.keysym.sym){
		        case SDLK_LEFT:
		            myDot.left();
		            break;
		        case SDLK_RIGHT:
		            myDot.right();
		            break;
		        case SDLK_UP:
		            myDot.up();
		            break;
		        case SDLK_DOWN:
		            myDot.down();
		            break;
		    }
    }
    return true;
}


int main(){

	SDL_Init(SDL_INIT_VIDEO);

    atexit(SDL_Quit);
    int screen_width;
    int screen_height;

    const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
    screen_width = info->current_w;
    screen_height = info->current_h;

   
    SDL_VideoModeOK(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);

    // Establecemos el modo de video
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);

    SDL_Surface *background = SDL_LoadBMP("background.bmp");

    printf("imagen de fondo = w:%i h:%i\n", background->w, background->h );

    SDL_Rect camera_pos = { 500, 500, screen_width, screen_height};
    Dot myDot(800,800);


    SDL_Event event;
    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;

    bool running=true;
    while(running ){

    	running = event_controller(event, myDot, camera_pos, screen_height, screen_width);
       //running = event_controller(event, myDot, camera_pos, screen_height, screen_width);
        //actualiza el dibujo de la superficie en la pantalla
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();


        if((t1 -t0) > 100) {
            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();

            //borro todo lo que estaba
            //toda la pantalla en negro
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
            
            //Set the camera_pos
        	myDot.follow(camera_pos, screen_height, screen_width);

        	keep_camera_pos_in_bounds(camera_pos,background->h,background->w +10);
        	SDL_BlitSurface(background, &camera_pos, screen, NULL);
        
        
        	//Show the dot on the screen
        	myDot.show(camera_pos, screen);
            

        }

    }
}






       
        
  