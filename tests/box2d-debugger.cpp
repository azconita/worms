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




class Worm{
public:
  b2Body* body;

  Worm(b2World* world){

    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position.Set(10, 8);
    body_def.bullet = false;
    body_def.userData = (void*) this;
    this->body = world->CreateBody(&body_def);
    b2PolygonShape shape;
    shape.SetAsBox(0.5, 1.6);
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 0;
    fixture.friction = 1;
    fixture.restitution = 0;
    this->body->CreateFixture(&fixture);
    this->body->SetUserData(this);
}

void left(){
  this->body->ApplyLinearImpulse(b2Vec2(-8,0), this->body->GetWorldCenter(), true);

}

void right(){
  this->body->ApplyLinearImpulse(b2Vec2(8,0), this->body->GetWorldCenter(), true);

}



  void show(SDL_Surface *screen){
    std::vector<b2Vec2> points;
    for (int i = 0; i < 4; i++){
      points.push_back(((b2PolygonShape*)this->body->GetFixtureList()->GetShape())->GetVertex(i) + this->body->GetWorldCenter());
    }

    /*for (int i = 0; i < 4; i++){
      printf("%f,%f\n",points[i].x,points[i].y );
    }*/
    

    Dot d0(23.5*10,23.5*10);
    Dot d1(23.5*points[0].x,23.5*points[0].y );
    Dot d2(23.5*points[1].x,23.5*points[1].y );
    Dot d3(23.5*points[2].x,23.5*points[2].y );
    Dot d4(23.5*points[3].x,23.5*points[3].y );

    d0.show(screen);
    d1.show(screen);
    d2.show(screen);
    d3.show(screen);
    d4.show(screen);

  }

};

class Viga{
public:
  b2Body* body ;

  Viga(b2World* world, int angulo){

  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(10, 10);
  bodyDef.angle = (M_PI / 180)*(180-angulo);
  this-> body = world->CreateBody(&bodyDef);
  b2PolygonShape shape;
  shape.SetAsBox(3, 0.4,b2Vec2(0, 0),(180-angulo)* (M_PI / 180));
 
  b2FixtureDef fixture;
  fixture.shape = &shape;
  fixture.friction = 3.0;
  fixture.density = 1;
  this->body->CreateFixture(&fixture);
}


void show(SDL_Surface *screen){
    std::vector<b2Vec2> points;
    for (int i = 0; i < 4; i++){
      points.push_back(((b2PolygonShape*)this->body->GetFixtureList()->GetShape())->GetVertex(i) + this->body->GetWorldCenter());
    }

    /*for (int i = 0; i < 4; i++){
      printf("%f,%f\n",points[i].x,points[i].y );
    }*/
    

    Dot d0(23.5*10,23.5*10);
    Dot d1(23.5*points[0].x,23.5*points[0].y );
    Dot d2(23.5*points[1].x,23.5*points[1].y );
    Dot d3(23.5*points[2].x,23.5*points[2].y );
    Dot d4(23.5*points[3].x,23.5*points[3].y );

    d0.show(screen);
    d1.show(screen);
    d2.show(screen);
    d3.show(screen);
    d4.show(screen);

  }


};


bool event_controller(SDL_Event &event, Worm & gusano){
  if(SDL_PollEvent(&event) != 1){
        return true; // no hay nuevos event
    }
    switch(event.type){
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:{
          switch(event.key.keysym.sym){
            case SDLK_ESCAPE:{
                return false;
            }
            case SDLK_LEFT:{
                gusano.left();
                break;
            }
            case SDLK_RIGHT:{
                gusano.right();
                break;
            }
          }
        }
    }
  
    return true;
}


int main(){

	b2Vec2 gravity(0, 9.8); //normal earth gravity
  b2World* world = new b2World(gravity);
  Viga viga(world, 0);
  Worm gusano(world);

  //////////////////////////////////

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

    	running = event_controller(event, gusano);
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();


        if((t1 -t0) > 100) {
            world->Step(0.1, 8, 3);
            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();

            //borro todo lo que estaba
            //toda la pantalla en negro
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
        	  SDL_BlitSurface(background, NULL, screen, NULL);
            viga.show(screen);
            gusano.show(screen);
        

  
    

        }

    }
	
}