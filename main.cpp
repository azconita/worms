#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <Box2D/Box2D.h>
#include "stage.h"
#include "Beam.h"
#include "Worm.h"

SDL_Surface* screen;
const float M2P=20;
const float P2M=1/M2P;

const int WIDTH=640;
const int HEIGHT=480;

void display(std::vector<Beam> &beams, std::vector<Worm> &worms);

int main(int argc, char const *argv[]) {
  Stage stage("stage1");
  SDL_Init(SDL_INIT_EVERYTHING);
  screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
  Uint32 start;
  SDL_Event event;
  bool running=true;
  while(running)
  {
          start=SDL_GetTicks();
          while(SDL_PollEvent(&event))
          {
                  switch(event.type)
                  {
                          case SDL_QUIT:
                                  running=false;
                                  break;
                          case SDL_KEYDOWN:
                                  switch(event.key.keysym.sym)
                                  {
                                          case SDLK_ESCAPE:
                                                  running=false;
                                                  break;
                                  }
                                  break;

                  }
          }
          stage.update();      //update
          std::vector<Beam> beams = stage.get_beams();
          std::vector<Worm> worms = stage.get_worms();
          display(beams, worms);
          SDL_Flip(screen);
          if(1000.0/30>SDL_GetTicks()-start)
                  SDL_Delay(1000.0/30-(SDL_GetTicks()-start));
  }
  SDL_Quit();
  return 0;
}

void putPixel(SDL_Surface* dest,int x,int y,int r,int g,int b) {
//      std::cout << x << " " << y << std::endl;
  if(x>=0 && x<dest->w && y>=0 && y<dest->h)
    ((Uint32*)dest->pixels)[y*dest->pitch/4+x]=SDL_MapRGB(dest->format,r,g,b);
}

void swapValue(int& a,int& b)
{
        int tmp=a;
        a=b;
        b=tmp;
}


void drawLine(SDL_Surface* dest,int x0,int y0,int x1,int y1)
{
        int tmp;
        bool step;

        step=abs(y1-y0)>abs(x1-x0);
        if(step)
        {
                swapValue(x0,y0);
                swapValue(x1,y1);
        }

        if(x0>x1)
        {
                swapValue(x1,x0);
                swapValue(y1,y0);
        }
        float error=0.0;
        float y=y0;
        float roundError=(float)abs(y1-y0)/(x1-x0);
        int ystep=(y1>y0 ? 1 : -1);
        for(int i=x0;i<x1;i++)
        {
                if(step)
                        putPixel(dest,y,i,255,255,255);
                else
                        putPixel(dest,i,y,255,255,255);
                error+=roundError;
                if(error>=0.5)
                {
                        y+=ystep;
                        error-=1;
                }
        }
}


void drawSquare(std::vector<b2Vec2> points) {
  for(int i=0;i<4;i++) {
    drawLine(screen, points[i].x*M2P, points[i].y*M2P,
        points[(i+1)>3 ? 0 : (i+1)].x*M2P,
        points[(i+1)>3 ? 0 : (i+1)].y*M2P);
  }
}

void display(std::vector<Beam> &beams, std::vector<Worm> &worms) {
  SDL_FillRect(screen,NULL,0);
  for (auto p: beams) {
    drawSquare(p.get_points());
  }
  for (auto p: worms) {
    drawSquare(p.get_points());
  }
}
