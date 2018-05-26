#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_video.h>
#include <cstdlib>
#include <tuple>
#include <utility>
#include <vector>

#include "stage.h"

SDL_Surface* screen;
const float M2P=20;
const float P2M=1/M2P;

const int WIDTH=640;
const int HEIGHT=480;

void display(StageDTO s);

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
                                          case SDLK_LEFT:
                                                  stage.make_action(0, 1);
                                                  break;
                                  }
                                  break;

                  }
          }
          stage.update();      //update
          StageDTO s = stage.get_stageDTO();
          display(s);
          //sleep(1);
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
        return true;
    }


};

/////////////////////////////////////////////////////////////////////////////////////////
class Animation_Factory{

private:
    Animation_Factory(){}
public:
    static Animation get_worm_walk(){
        Color colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);
        Animation worm(WORM_WALK,colorkey,WORM_WALK_COLUMNS,WORM_WALK_ROWS);
        return worm;
    }

    static Animation get_worm_jump(){
        Color colorkey(WORM_JUMP_R,WORM_JUMP_G,WORM_JUMP_B);
        Animation worm(WORM_JUMP,colorkey,WORM_JUMP_COLUMNS,WORM_JUMP_ROWS);
        return worm;

    }

    static Animation get_worm_fall(){
        Color colorkey(WORM_FALL_R,WORM_FALL_G,WORM_JUMP_B);
        Animation worm(WORM_FALL,colorkey,WORM_FALL_COLUMNS,WORM_FALL_ROWS);
        return worm;

    }


};

/////////////////////////////////////////////////////////////////////////////////////////



class Worm_Animation_Controller{
public:
    int x, y;
    State state;
    std::map<int,Animation> animations;


Worm_Animation_Controller(int initial_x, int initial_y){
    this->x = initial_x;
    this-> y = initial_y;
    this->state = Still;
    Animation worm_walk = Animation_Factory::get_worm_walk();

    this->animations.insert(std::pair<int,Animation>(Still,worm_walk));
    this->animations.insert(std::pair<int,Animation>(Walk,worm_walk));

    Animation worm_fall = Animation_Factory::get_worm_fall();
    this->animations.insert(std::pair<int,Animation>(Fall,worm_fall));

    Animation worm_jump = Animation_Factory::get_worm_jump();
    this->animations.insert(std::pair<int,Animation>(Jump,worm_jump));
}

Direction get_direction(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    return animation_iter->second.get_current_direction();
}

void wish_to_move(State state, Direction direction){
    for(std::map<int,Animation>::iterator animation_iter = this->animations.begin();animation_iter != this->animations.end() ;animation_iter ++){
        animation_iter->second.set_current_direction(direction);
    }
    this->state = state;
    //std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    //animation_iter->second
}

void wish_to_move(State state){
    Direction last_direction = get_direction();
    wish_to_move(state, last_direction);
}

void move(int position_x, int position_y){
    if(this->state == Walk && position_y > this->y){ //aumenta el y, se cae
        this->state = Fall;
    }
    this->x = position_x;
    this->y = position_y;
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);

    if(this->state != Still){
        if(!animation_iter->second.continue_internal_movement()){
            this->state = Still;
        }
    }

}

void show(SDL_Surface * screen){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    animation_iter->second.draw(screen, this->x, this->y);
}

};

/////////////////////////////////////////////////////////////////////////////////////////

float get_pixels(float meter_position){
    return  23.5*meter_position;
}

void debug_box2d_figure(SDL_Surface *screen, std::vector<std::tuple<float, float>> vertices){

    int i = 0;
    for(auto vertice: vertices){
        int x = std::get<0>(vertice);
        int y = std::get<1>(vertice);
        //printf("v %i = x:%i, y:%i\n", i,x, y );
        i +=1;
    }


    std::tuple<float, float> up_left_vertex = vertices[0];
    int up_left_vertex_x = std::get<0>(up_left_vertex);
    int up_left_vertex_y = std::get<1>(up_left_vertex);

    std::tuple<float, float> down_right_vertex = vertices[2];
    int down_right_vertex_x = std::get<0>(down_right_vertex);
    int down_right_vertex_y = std::get<1>(down_right_vertex);

    //dibujo un rectangulo
    SDL_Rect rectangle;
    rectangle.x = get_pixels(up_left_vertex_x);
    rectangle.y = get_pixels(up_left_vertex_y);
    rectangle.h = get_pixels(down_right_vertex_y - up_left_vertex_y);
    rectangle.w = get_pixels(down_right_vertex_x - up_left_vertex_x);

    //printf("rectangle = x : %i y: %i h: %i w: %i\n",rectangle.x,rectangle.y,rectangle.h,rectangle.w );

    Uint32 colorkey = SDL_MapRGBA(screen->format, 0, 255, 0, 5);
    SDL_FillRect(screen, &rectangle, colorkey);

}


void show_beams(StageDTO s, SDL_Surface *screen){

    Color colorkey_beam(BEAM_R,BEAM_G,BEAM_B);

    for (auto b: s.beams) {

        std::vector<std::tuple<float, float>> vertices = b.second;

        //debug_box2d_figure(screen, vertices);

        std::tuple<float, float> up_left_vertex = vertices[0];
        int up_left_vertex_x = get_pixels(std::get<0>(up_left_vertex));
        int up_left_vertex_y = get_pixels(std::get<1>(up_left_vertex));


        Picture beam(BEAM, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        beam.draw(screen,up_left_vertex_x, up_left_vertex_y);

    }

}

std::map<int,Worm_Animation_Controller> create_worms(StageDTO s, SDL_Surface *screen){

    std::map<int,Worm_Animation_Controller> worms;

    Color colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);

    for (auto w: s.worms) {

        int id = w.first;
        std::vector<std::tuple<float, float>> vertices = w.second;

        std::tuple<float, float> up_left_vertex = vertices[0];
        int position_worm_x = get_pixels(std::get<0>(up_left_vertex));
        int position_worm_y = get_pixels(std::get<1>(up_left_vertex));

        //creo el gusano y lo gardo en el vector
        Worm_Animation_Controller worm(position_worm_x, position_worm_y);
        worms.insert(std::pair<int,Worm_Animation_Controller>(id,worm));
        worm.show(screen);

    }

    return worms;
}




void show_worms(StageDTO s, SDL_Surface *screen, std::map<int,Worm_Animation_Controller> & worms){

    for (auto w: s.worms) {

        std::vector<std::tuple<float, float>> vertices = w.second;

        debug_box2d_figure(screen, vertices);

        std::tuple<float, float> up_left_vertex = vertices[0];
        int up_left_vertex_x = get_pixels(std::get<0>(up_left_vertex));
        int up_left_vertex_y = get_pixels(std::get<1>(up_left_vertex));

        std::map<int,Worm_Animation_Controller>::iterator worms_iter = worms.find(w.first);
        worms_iter->second.move(up_left_vertex_x, up_left_vertex_y);
        worms_iter->second.show(screen);

    }
}
//////////////////////////////////////////////////////////////////////////////////////////
class Event_Controller{
    Stage & stage;
    SDL_Event &  event;
public:
    Event_Controller(SDL_Event & event, Stage & stage):
        event(event),
        stage(stage)
         {}

bool continue_running(Worm_Animation_Controller& turn_worm){
    SDL_PollEvent(&this->event);
    switch(event.type){
        case SDL_QUIT:
            cout << "se apreto x -> fin" << endl;
            return false;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    return false;

                case SDLK_LEFT:
                    cout << "se apreto izquierda " << endl;
                    turn_worm.wish_to_move(Walk,Left);
                    this->stage.make_action(0,LEFT);
                    break;
                case SDLK_RIGHT:
                    cout << "se apreto derecha " << endl;
                    turn_worm.wish_to_move(Walk,Right);
                    this->stage.make_action(0,RIGHT);
                    break;
                case SDLK_UP:
                    cout << "se apreto arriba " << endl;
                    turn_worm.wish_to_move(Jump);
                    this->stage.make_action(0,UP);
                    break;
                }
        }
}

void drawSquare(std::vector<std::tuple<float,float>> points) {
  for(int i=0;i<4;i++) {
    float x, y, x1, y1;
    std::tie (x,y) = points[i];
    std::tie (x1,y1) = points[(i+1)>3 ? 0 : (i+1)];
    drawLine(screen, x*M2P, y*M2P, x1*M2P, y1*M2P);
    //drawLine(screen, x, y, x1, y1);
  }
}

void display(StageDTO s) {
  SDL_FillRect(screen,NULL,0);
  for (auto p: s.beams) {
    drawSquare(p.second);
  }
  for (auto p: s.worms) {
    drawSquare(p.second);
  }
}
