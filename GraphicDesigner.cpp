/*
 * GraphicDesigner.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

#include "GraphicDesigner.h"



float GraphicDesigner::get_pixels(float meter_position){
    return  23.5*meter_position;
}


std::map<int,WormAnimation> GraphicDesigner::create_worms(StageDTO s){

    std::map<int,WormAnimation> worms;

    for (auto w: s.worms) {

        int id = w.first;
        ElementDTO worm_info = w.second;

        int position_worm_x = get_pixels(worm_info.pos_x);
        int position_worm_y = get_pixels(worm_info.pos_y);

        Direction dir = Left;
        if(worm_info.player_id %2 == 0){
            dir = Right;
        }
        WormAnimation worm(position_worm_x, position_worm_y, dir, worm_info.player_id, id);
        worms.insert(std::pair<int,WormAnimation>(id,worm));
    }
    return worms;
}


bool GraphicDesigner::is_timer_weapon(Weapon_Name weapon){
    return std::find(weapons_with_timer.begin(), weapons_with_timer.end(), weapon) != weapons_with_timer.end();
}



GraphicDesigner::GraphicDesigner(SDL_Surface * screen, int screen_height, int screen_width,StageDTO initial_stage):
water(3){
    this->screen = screen;
    this->screen_height = screen_height;
    this->screen_width = screen_width;

    if (TTF_Init() != 0) {
      throw Error("TTF_Init() Failed: ",TTF_GetError());
    }
    this->font = TTF_OpenFont(FONT_FILE, FONT_SIZE);
    this->time_font = TTF_OpenFont(TIMER_FONT, TIMER_SIZE);
    if (this->font == NULL ||  this->time_font == NULL){
        throw Error("TTF_OpenFont() Fail: ",TTF_GetError());
    }

 
    SDL_Surface *power_bar = IMG_Load(POWER_BAR);
    SDL_Surface *power_back = IMG_Load(POWER_BACK);;
    SDL_Surface * weapons_menu = IMG_Load(WEAPONS_MENU);
    SDL_Surface * background = IMG_Load(BACKGROUND);
    SDL_Surface * arrow = IMG_Load(ARROW);
    SDL_Surface *winner = IMG_Load(WON);
    SDL_Surface *looser = IMG_Load(LOST);

      printf("[GraphicDesigner] 3\n");
  
    if (!power_bar || !power_back|| !weapons_menu || !background || !arrow  || !winner || !looser) {
        throw Error("Couldn't create surface from image:  %s",SDL_GetError());
    }
    ////////////////////////////////////////////////////////

    this->background = background;
    this->camera = new Camera(this->screen_height, this->screen_width);

    this->worms = create_worms(initial_stage);
    this->weapons = AnimationFactory::get_weapons();

    this->little_beams =  AnimationFactory::get_little_beams();
    this->big_beams = AnimationFactory::get_big_beams();

    this->power_bar = power_bar;
    this->power_back = power_back;
    this->weapons_menu = weapons_menu;
    this->arrow = arrow;

    this->winner = winner;
    this->looser = looser;
    this->last_worm_turn = INTER_TURN;

}

void GraphicDesigner::resize(int h, int w){
    this->screen_height = h;
    this->screen_width = w;
}

void GraphicDesigner::scroll(int x, int y){
    if(x < 15){
        this->camera->move(-10,0);
        return;
    }
    if(y < 15){
        this->camera->move(0,-10); // esto no estaria funcionando
        return;
    }
    if(x >this->screen_width -15){
       this->camera->move(10,0);
        return;
    }
    if(y >this->screen_height -15){
        this->camera->move(0,10);
       return;
    }
}


void GraphicDesigner::show_elements(StageDTO s, SDL_Surface *screen){
    SDL_Rect camera_position = this->camera->get_focus();
    SDL_BlitSurface(this->background, &camera_position, this->screen, NULL);
    this->water.show(screen, this->background->h - camera_position.y);
    this->show_beams(s,screen,camera_position);
    this->show_worms(s,screen,camera_position);
    this->show_weapon(s,screen,camera_position);
    this->show_weapons_menu(s);
}


void GraphicDesigner::show_beams(StageDTO s, SDL_Surface *screen, SDL_Rect camera_position){

    for (auto beam_info: s.beams) {

        int center_x = get_pixels(beam_info.pos_x) - camera_position.x;
        int center_y = get_pixels(beam_info.pos_y) - camera_position.y;

        float degrees = beam_info.angle;
        if(beam_info.direction == Left){
            degrees = 180-degrees;
        }


        if(beam_info.w < 4){
            Picture beam = inclinate_beam(this->little_beams, degrees);
            beam.draw(this->screen,center_x - 1, center_y,beam_info.direction);
        }else{
            Picture beam = inclinate_beam(this->big_beams, degrees);
             beam.draw(this->screen,center_x , center_y, beam_info.direction);
        } 
    }
}



Picture GraphicDesigner::inclinate_beam(std::vector<Picture> beams, float degrees){

    if(beams.size() < 5){
        throw Error("[GraphicDesigner] No se cargaron todas las imagenes de las vigas");
    }
    if(degrees < 5){
        Picture beam = beams[0];
        return beam;
    }
    if(degrees < 33){
        Picture beam = beams[1];
        return beam;
    }
    if(degrees < 55){
        Picture beam = beams[2];
        return beam;
    }
     if(degrees < 80){
        Picture beam = beams[3];
        return beam;
    }
    Picture beam = beams[4];
    return beam;
    

}


void GraphicDesigner::show_worms(StageDTO s, SDL_Surface *screen, SDL_Rect camera_position){
    for (auto w: s.worms) {
        std::map<int,WormAnimation>::iterator worms_iter = this->worms.find(w.first);
        
        ElementDTO worm_info = w.second;
        int center_x = get_pixels(worm_info.pos_x);
        int center_y = get_pixels(worm_info.pos_y);

        worms_iter->second.move(center_x, center_y,  worm_info.worm_state,worm_info.direction);


        if(w.first == s.worm_turn && ((this->last_worm_turn != s.worm_turn) || //
            (worms_iter->second.is_in_movement() && s.weapons.size() == 0 ) ) ) { 
            this->camera->follow(center_x,center_y); 
        }
        worms_iter->second.show(this->screen, camera_position);


        if(worm_info.player_id > 4){
            cout << "Error: juego no preparado para mas de 4 jugadores" << endl;
        }

        center_x = center_x - camera_position.x;
        center_y = center_y - camera_position.y;

        
        Colour player_color = Colour::create(possible_colors.at(worm_info.player_id));

        show_life(worm_info.life,center_x,center_y,player_color);
        if(w.first == s.worm_turn){
            show_arrow(center_x, center_y);
        }
        float weapon_power = worms_iter->second.get_weapon_power();
        if(weapon_power > 0){
            show_powerbar(weapon_power);
        }
        
        if(worms_iter->second.has_point_weapon()){
            show_weapon_point_direction(center_x, center_y,worms_iter->second.get_degrees(), worms_iter->second.get_direction());
        }

    }
}

void GraphicDesigner::show_weapon_point_direction(int x, int y,float degrees, Direction dir){
    int val = 1;
    if(dir == Left){
        printf("[GraphicDesigner]Left\n");   
        val = -1 ;
    }
    Colour color = Colour::create(White);
    Uint32 colorkey = SDL_MapRGB(screen->format,color.r,color.g,color.b);
    SDL_Rect dot;
    for(int hipo = 50; hipo < 300; hipo += 35){
        dot.x = x  + 25 + val*( cos(degrees* (M_PI / 180))*hipo);
        dot.y = y  + 20 -       sin(degrees* (M_PI / 180))*hipo;
        dot.h = 3;
        dot.w = 3;
        SDL_FillRect(screen, &dot, colorkey);
    }  
}

 std::map<int,WormAnimation>::iterator GraphicDesigner::get_turn_worm(int id){
   return this->worms.find(id);
 }


 void GraphicDesigner::show_weapon( StageDTO s,SDL_Surface * screen, SDL_Rect camera_position){

    for (auto w: s.weapons) {
        int padding_y = 10;
        if(w.weapon == Explosion){
            padding_y = 35;

        }
        int center_x = get_pixels(w.pos_x) - camera_position.x;
        int center_y = get_pixels(w.pos_y) - camera_position.y - padding_y;


        std::map<Weapon_Name,Animation>::iterator weapon_iter = this->weapons.find(w.weapon);
        weapon_iter->second.add_degres(w.angle);
        weapon_iter->second.draw(this->screen,center_x, center_y);

        if(this->last_worm_turn == s.worm_turn){
             this->camera->follow(get_pixels(w.pos_x) - ((float) weapon_iter->second.get_width()/2),//
                             get_pixels(w.pos_y) - ((float) weapon_iter->second.get_height()/2));
        } else {
            this->last_worm_turn = s.worm_turn;
        }
       
        if(is_timer_weapon(w.weapon)){
            show_timer(w.timer);
        }
        if (w.weapon == W_Fragment)
          printf("fragment: pos: %f,%f\n", w.pos_x, w.pos_y);
    }

}


void GraphicDesigner::show_life(int life, int worm_x, int worm_y, Colour color){
    char str_life[10];
    if(life < 100){
        sprintf(str_life, " %d ", life);
    }else{
        sprintf(str_life, "%d", life);
    }

    SDL_Color black_text_color = {0,0,0};
    SDL_Surface * text = TTF_RenderText_Solid(font,str_life,black_text_color);
    if (text == NULL){
        throw Error("TTF_RenderText_Solid(): ",TTF_GetError());
    }

    Sint16 x = worm_x  + 30;
    Sint16 y = worm_y  - 10;

    SDL_Rect rectangle; 
    rectangle.x = x-2;
    rectangle.y = y-2;
    rectangle.h = text->h + 2;
    rectangle.w = text->w + 4 ;
    Uint32 colorkey = SDL_MapRGBA(this->screen->format, color.r, color.g, color.b,0.2);
    SDL_FillRect(this->screen, &rectangle, colorkey);

    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = text->h;
    dimention.w = text->w;

    SDL_Rect position;
    position.x = x; 
    position.y = y;
    position.h = text->h;
    position.w = text->w;
    SDL_BlitSurface(text, &dimention, this->screen, &position);
    SDL_FreeSurface(text);
}

void GraphicDesigner::show_arrow( int worm_x, int worm_y){

    Sint16 x = worm_x + 5;
    Sint16 y = worm_y  - 18;

    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = this->arrow->h;
    dimention.w = this->arrow->w;

    SDL_Rect position;
    position.x = x;
    position.y = y;
    position.h = this->arrow->h;
    position.w = this->arrow->w;
    
    Colour color = Colour::create(White);
    Uint32 colorkey = SDL_MapRGB(this->arrow->format, color.r, color.g, color.b);
    SDL_SetColorKey(this->arrow, SDL_SRCCOLORKEY, colorkey);
    SDL_BlitSurface(this->arrow, &dimention, this->screen, &position);
}

void GraphicDesigner::show_powerbar(float power){
    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = this->power_bar->h;
    dimention.w = this->power_bar->w;

    SDL_Rect position;
    position.x = this->screen_width - this->power_bar->w -5;
    position.y = 5;
    position.h = this->power_bar->h;
    position.w = this->power_bar->w;

    Colour color = Colour::create(White);
    Uint32 colorkey = SDL_MapRGB(this->power_back->format, color.r, color.g, color.b);
    SDL_SetColorKey(this->power_back, SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey(this->power_bar, SDL_SRCCOLORKEY, colorkey);
    SDL_BlitSurface(this->power_back, &dimention, this->screen, &position);


    dimention.x = 0;
    dimention.y = 0;
    dimention.h = this->power_bar->h;
    dimention.w = this->power_bar->w*power/100;
    SDL_BlitSurface(this->power_bar, &dimention, this->screen, &position);


}

void GraphicDesigner::show_weapons_menu(StageDTO& s){
    SDL_Rect position;
    position.x = this->screen_width - this->weapons_menu->w;
    position.y = this->screen_height/2- this->weapons_menu->h/2;
    position.h = this->weapons_menu->h;
    position.w = this->weapons_menu->w;

    SDL_Rect dimention;
    dimention.x = 0;    
    dimention.y = 0;
    dimention.h = this->weapons_menu->h;
    dimention.w = this->weapons_menu->w;
    SDL_BlitSurface(this->weapons_menu, &dimention, this->screen, &position);  
}


bool GraphicDesigner::is_inside_weapon_menu(int x, int y){
    int x_relative = x -(this->screen_width - this->weapons_menu->w);
    int y_relative = y - (this->screen_height/2- this->weapons_menu->h/2);
    if(x_relative < 0 || y_relative > this->weapons_menu->h){
        return false;
    }
    printf("[GraphicDesigner] se hizo click en el menu\n");
    return true;
}


Weapon_Name GraphicDesigner::choose_weapon(int x, int y){
    int icon_y = (this->weapons_menu->h/10); //longitud de un icono
    int menu_init = (this->screen_height/2- this->weapons_menu->h/2); 
    if(y < menu_init + icon_y){
        return W_Air_Attack;
    }
    if(y < menu_init + icon_y*2){
        return Baseball_Bat;
    }
    if(y < menu_init + icon_y*3 ){
        return W_Bazooka;
    }
    if(y < menu_init + icon_y*4){
        return Red_Grenade;
    }
    if(y < menu_init + icon_y*5){
        return Green_Grenade;
    }
    if(y < menu_init + icon_y*6){
        return Holy_Grenade;
    }
    if(y < menu_init + icon_y*7){
        return Mortar;
    }
    if(y < menu_init + icon_y*8){
        return Teleport;
    }
    if(y < menu_init + icon_y*9){
        return Dynamite;
    }
    return Banana;

}

void GraphicDesigner::show_timer(int second){
    char time[10];
    sprintf(time, "00:00:0%d ", second);

    SDL_Color red_text_color = {255,0,0};
    SDL_Surface * text = TTF_RenderText_Solid(time_font,time,red_text_color);
    if (text == NULL){
        throw Error("TTF_RenderText_Solid(): ",TTF_GetError()); 
    }

    SDL_Rect white_rec;
    white_rec.x = this->screen_width/2 - 6;
    white_rec.y = 25 - 4;
    white_rec.h = text->h + 8;
    white_rec.w = text->w + 8;
    Uint32 withe  = SDL_MapRGBA(this->screen->format, 255, 255,255,100);
    SDL_FillRect(this->screen, &white_rec, withe);

    SDL_Rect rectangle;
    rectangle.x = this->screen_width/2 - 2;
    rectangle.y = 25;
    rectangle.h = text->h;
    rectangle.w = text->w;
    Uint32 colorkey = SDL_MapRGBA(this->screen->format, 0, 0, 0, 0);
    SDL_FillRect(this->screen, &rectangle, colorkey);

    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = text->h;
    dimention.w = text->w;

    SDL_Rect position;
    position.x = this->screen_width/2;
    position.y = 25;
    position.h = text->h;
    position.w = text->w;
    SDL_BlitSurface(text, &dimention, this->screen, &position);
    //SDL_FreeSurface(text);
}

SDL_Rect GraphicDesigner::get_camera_position(){
    return this->camera->get_focus(); 
}

void GraphicDesigner::won(){
    SDL_Rect camera_position = this->camera->get_focus();
    SDL_FillRect(this->background, NULL, SDL_MapRGB(this->screen->format,128,128,192));

    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = this->winner->h;
    dimention.w = this->winner->w;

    SDL_Rect position;
    position.x =  screen_width/2  - this->winner->w/2;
    position.y =  screen_height/2 - this->winner->h/2;
    position.h = this->winner->h;
    position.w = this->winner->w;
    SDL_BlitSurface(this->winner, &dimention, this->screen, &position);
}
 
void GraphicDesigner::lost(){
    SDL_FillRect(this->background, NULL, SDL_MapRGB(this->screen->format,128,128,192));
    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = this->looser->h;
    dimention.w = this->looser->w;

    SDL_Rect position;
    position.x =  screen_width/2 - this->looser->w/2;
    position.y =  screen_height/2 - this->looser->h/2;
    position.h = this->looser->h;
    position.w = this->looser->w;
    SDL_BlitSurface(this->looser, &dimention, this->screen, &position);
}


GraphicDesigner::~GraphicDesigner() {
	delete(this->camera);
    SDL_FreeSurface(this->background);
    SDL_FreeSurface(this->power_bar);
    SDL_FreeSurface(this->weapons_menu);
    SDL_FreeSurface(this->arrow);
    TTF_Quit();
}


