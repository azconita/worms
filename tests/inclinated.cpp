#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include <Box2D/Box2D.h>

#include <iostream>
#include <vector>
#include <math.h>



#define DOT_WIDTH 5
#define DOT_HEIGHT 5


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
    
    //Shows the dot on the screen
    void show( SDL_Surface *screen){
    	//Show the dot
    	SDL_Rect rectangle;
    	rectangle.x = x;
    	rectangle.y = y;
    	rectangle.h = DOT_HEIGHT;
    	rectangle.w = DOT_WIDTH; 
    	Uint32 colorkey = SDL_MapRGB(screen->format, 255,0,0);
    	SDL_FillRect(screen, &rectangle, colorkey);
    	
    }
    

};




bool event_controller(SDL_Event &event){
	if(SDL_PollEvent(&event) != 1){
        return true; // no hay nuevos event
    }
    switch(event.type){
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE ){
                return false;
            }
       
	}
  
    return true;
}



int main(){

	 b2Vec2 gravity(0, 9.8); //normal earth gravity
  b2World* world = new b2World(gravity);


  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(10, 10);
  bodyDef.angle = (M_PI / 180)*(180-22);
  b2Body* body = world->CreateBody(&bodyDef);
  b2PolygonShape shape;
  shape.SetAsBox(3, 0.4,b2Vec2(0, 0),(180-22)* (M_PI / 180));
 
  b2FixtureDef fixture;
  fixture.shape = &shape;
  fixture.friction = 3.0;
  fixture.density = 1;
  body->CreateFixture(&fixture);

   std::vector<b2Vec2> points;
  for (int i = 0; i < 4; i++){
    points.push_back(((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(i) + body->GetWorldCenter());
  }

     for (int i = 0; i < 4; i++){
      printf("%f,%f\n",points[i].x,points[i].y );

    }
    Dot d0(23.5*10,23.5*10);
    Dot d1(23.5*points[0].x,23.5*points[0].y );
    Dot d2(23.5*points[1].x,23.5*points[1].y );
    Dot d3(23.5*points[2].x,23.5*points[2].y );
    Dot d4(23.5*points[3].x,23.5*points[3].y );


/*  b2BodyDef bodyDef2;
  bodyDef2.type = b2_dynamicBody;
  bodyDef2.position.Set(10, 5);
  bodyDef2.bullet = true;
   b2Body*  body2 = world->CreateBody(&bodyDef);


  b2PolygonShape shape2;
  shape2.SetAsBox(1, 1);
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &shape2;
  myFixtureDef.density = 4;
  body2->CreateFixture(&myFixtureDef);


  for (int i = 0; i < 4; i++){
    points.push_back(((b2PolygonShape*)body2->GetFixtureList()->GetShape())->GetVertex(i) + body2->GetWorldCenter());
  }

  SDL_Rect rectangle;
   rectangle.x = position[0].x;
    	rectangle.y = position[0].y;
    	rectangle.h = 5;
    	rectangle.w = 5;
    	Uint32 colorkey = SDL_MapRGB(screen->format, 255,255,0);
    	SDL_FillRect(screen, &rectangle, colorkey);



  */

 


  	SDL_Init(SDL_INIT_VIDEO);

    atexit(SDL_Quit);
   
    SDL_VideoModeOK(900, 900, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);

    // Establecemos el modo de video
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(900, 900, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);

    SDL_Surface *background = SDL_LoadBMP("background.bmp");


    SDL_Event event;
    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;

    bool running=true;
    while(running ){

    	running = event_controller(event);
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();


        if((t1 -t0) > 100) {
            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();

            //borro todo lo que estaba
            //toda la pantalla en negro
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
        	SDL_BlitSurface(background, NULL, screen, NULL);
        

    		//Show the dot
    		d0.show(screen);
    		    	 	d1.show(screen);
    	 	d2.show(screen);
    	 	d3.show(screen);
    	 	d4.show(screen);

    

        }

    }
	
}