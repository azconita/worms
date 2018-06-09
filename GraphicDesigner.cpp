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

    printf("creador de gusanos\n");

    std::map<int,WormAnimation> worms;

    for (auto w: s.worms) {

        int id = w.first;
        ElementDTO worm_info = w.second;

        int position_worm_x = get_pixels(worm_info.pos_x);
        int position_worm_y = get_pixels(worm_info.pos_y);

        //creo el gusano y lo gardo en el vector
        Direction dir = Left;
        if(worm_info.player_id %2 == 0){
            dir = Right;
        }
        WormAnimation worm(position_worm_x, position_worm_y, dir);
        worms.insert(std::pair<int,WormAnimation>(id,worm));
    }
    return worms;
}


bool GraphicDesigner::is_timer_weapon(Weapon_Name weapon){
    return std::find(weapons_with_timer.begin(), weapons_with_timer.end(), weapon) != weapons_with_timer.end();
}



GraphicDesigner::GraphicDesigner(SDL_Surface * screen, int screen_height, int screen_width,StageDTO initial_stage){
    this->screen = screen;
    this->screen_height = screen_height;
    this->screen_width = screen_width;

    if (TTF_Init() != 0) {
      cout << "TTF_Init() Failed: " << TTF_GetError() << endl;
      SDL_Quit();
      exit(1);
    }
    this->font = TTF_OpenFont(FONT_FILE, FONT_SIZE);
    if (this->font == NULL){
        cout << "TTF_OpenFont() Fail: " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    this->time_font = TTF_OpenFont(TIMER_FONT, TIMER_SIZE);
    if (this->time_font == NULL){
        cout << "TTF_OpenFont() Fail: " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }


    SDL_Surface *power_bar = IMG_Load(POWER_BAR);
    if (!power_bar) {
        cout <<"Couldn't create surface from image:" << POWER_BAR << SDL_GetError() << endl;
        return;
    }
    Uint32 colorkey = SDL_MapRGB(power_bar->format, 0, 0, 0);
    SDL_SetColorKey(power_bar, SDL_SRCCOLORKEY, colorkey);
    this->power_bar = power_bar;

    SDL_Surface * weapons_menu = IMG_Load(WEAPONS_MENU);
    if (!power_bar) {
        cout <<"Couldn't create surface from image:" << WEAPONS_MENU << SDL_GetError() << endl;
        return;
    }
    this->weapons_menu = weapons_menu;

    ////////////////////////////////////////////////////////

    SDL_Surface *background = SDL_LoadBMP(BACKGROUND);
    Sint16 x = background->w/2 - this->screen_height/2;
    Sint16 y = background->h/2 -this->screen_height/2;
    Uint16 h = this->screen_height;
    Uint16 w = this->screen_width;
    this->camera = { x,y, w, h};

    this->worms = create_worms(initial_stage);
    this->weapons = AnimationFactory::get_weapons();

    this->little_beams =  AnimationFactory::get_little_beams();
    this->big_beams = AnimationFactory::get_big_beams();

}

void GraphicDesigner::scroll(int x, int y){
    printf("moviendooo en x=%i y = %i\n",x, y);
    printf("H: %i W: %i\n",this->camera.y, this->camera.x );
    if(x < 15){
        this->camera.x -=10;
    }
    if(y < 15){
        printf("se deberia mover para arriba\n");
        this->camera.y -=10;
        printf("camara y\n", this->camera.y);
    }
    if(x > this->camera.x -15){
        this->camera.x += 10;
    }
    if(y > this->camera.y -15){
        this->camera.y += 10;
    }
}

void GraphicDesigner::show_background(){
    SDL_Surface *background = SDL_LoadBMP(BACKGROUND);
    SDL_BlitSurface(background, &this->camera, this->screen, NULL);
}


void GraphicDesigner::show_beams(StageDTO s, SDL_Surface *screen){
    

    for (auto beam_info: s.beams) {

        //debug_box2d_figure(screen, beam_info);

        int up_left_vertex_x = get_pixels(beam_info.pos_x) - this->camera.x;
        int up_left_vertex_y = get_pixels(beam_info.pos_y) - this->camera.y;


        if(beam_info.w == 3){
            Picture beam = inclinate_beam(this->little_beams, beam_info.angle);
            beam.draw(this->screen,up_left_vertex_x, up_left_vertex_y);
        }else{
            Picture beam = inclinate_beam(this->big_beams, beam_info.angle);
             beam.draw(this->screen,up_left_vertex_x, up_left_vertex_y);
        } 
    }
}

Picture GraphicDesigner::inclinate_beam(std::vector<Picture> beams, float degree){
    if(beams.size() < 5){
        throw Error("No se cargaron todas las imagenes de las vigas");
    }
    if(degree < 5){
        Picture beam = beams[0];
        return beam;
    }
    if(degree < 33){
        Picture beam = beams[1];
        return beam;
    }
    if(degree < 55){
        Picture beam = beams[2];
        return beam;
    }
     if(degree < 80){
        Picture beam = beams[3];
        return beam;
    }
    Picture beam = beams[4];
    return beam;
    

}


void GraphicDesigner::show_worms(StageDTO s, SDL_Surface *screen){

    for (auto w: s.worms) {

        ElementDTO worm_info = w.second;

        //debug_box2d_figure(screen, worm_info);

        int up_left_vertex_x = get_pixels(worm_info.pos_x) - this->camera.x;
        int up_left_vertex_y = get_pixels(worm_info.pos_y) - this->camera.y;

        std::map<int,WormAnimation>::iterator worms_iter = this->worms.find(w.first);
        worms_iter->second.move(up_left_vertex_x, up_left_vertex_y);
        worms_iter->second.show(this->screen);

        if(worm_info.player_id > 4){
            cout << "Error: juego no preparado para mas de 4 jugadores" << endl;
        }

        Colour player_color = Colour::create(possible_colors.at(worm_info.player_id));

        show_life(worm_info.life,up_left_vertex_x+20,up_left_vertex_y-5,player_color);
        int weapon_power = worms_iter->second.get_weapon_power();
        show_powerbar(weapon_power);

    }
}

 std::map<int,WormAnimation>::iterator GraphicDesigner::get_turn_worm(int id){
   return this->worms.find(id);
 }


 void GraphicDesigner::show_weapon( StageDTO s,SDL_Surface * screen){
    for (auto w: s.weapons) {

        //debug_box2d_figure(screen, w);

        int up_left_vertex_x = get_pixels(w.pos_x) - this->camera.x;
        int up_left_vertex_y = get_pixels(w.pos_y) - this->camera.y;


        std::map<Weapon_Name,Animation>::iterator weapon_iter = this->weapons.find(w.weapon);
        weapon_iter->second.continue_internal_movement();
        weapon_iter->second.draw(this->screen,up_left_vertex_x, up_left_vertex_y);

        if(is_timer_weapon(w.weapon)){
            show_timer(w.timer);
        }
    }

}





void GraphicDesigner::show_life(int life, int x, int y, Colour color){

    char str_life[10];
    if(life < 100){
        sprintf(str_life, " %d ", life);
    }else{
        sprintf(str_life, "%d", life);
    }

    SDL_Color black_text_color = {0,0,0};
    SDL_Surface * text = TTF_RenderText_Solid(font,str_life,black_text_color);
    if (text == NULL){
        cout << "TTF_RenderText_Solid(): " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

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


}

void GraphicDesigner::show_powerbar(int power){
    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = this->power_bar->h;
    dimention.w = this->power_bar->w*power/100;

    SDL_Rect position;
    position.x = this->screen_width - this->power_bar->w -5;
    position.y = 5;
    position.h = this->power_bar->h;
    position.w = this->power_bar->w;

    Colour color = Colour::create(Black);
    Uint32 colorkey = SDL_MapRGB(this->power_bar->format, color.r, color.g, color.b);
    SDL_SetColorKey(this->power_bar, SDL_SRCCOLORKEY, colorkey);
    SDL_BlitSurface(this->power_bar, &dimention, this->screen, &position);


}

void GraphicDesigner::show_weapons_menu(int size){
    SDL_Rect dimention;
    dimention.x = 0;
    dimention.y = 0;
    dimention.h = this->weapons_menu->h;
    dimention.w = this->weapons_menu->w*size/100;

    SDL_Rect position;
    position.x = this->screen_width - this->weapons_menu->w -5;
    position.y = 5;
    position.h = this->weapons_menu->h;
    position.w = this->weapons_menu->w;
    SDL_BlitSurface(this->weapons_menu, &dimention, this->screen, &position);
}

bool GraphicDesigner::is_inside_weapon_menu(int x, int y){
    int x_relative = x -(this->screen_width - this->weapons_menu->w -5);
    int y_relative = y - 5;
    if(x_relative < 0 || y_relative > this->weapons_menu->h){
        return false;
    }
    return true;
}


Weapon_Name GraphicDesigner::choose_weapon(int x, int y){

    int icon_y = this->weapons_menu->h/10;
    if(y < icon_y){
        return W_Air_Attack;

    }
    if(y < icon_y*2){
        return Baseball_Bat;
    }
    if(y < icon_y*3){
        return W_Bazooka;
    }
    if(y < icon_y*4){
        return Red_Grenade;
    }
    if(y < icon_y*5){
        return Green_Grenade;
    }
    if(y < icon_y*6){
        return Holy_Grenade;

    }
    if(y < icon_y*7){
        return Mortar;

    }
    if(y < icon_y*8){
        return Teleport;
    }
    if(y < icon_y*9){
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
        cout << "TTF_RenderText_Solid(): " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
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


}



GraphicDesigner::~GraphicDesigner() {
	// TODO Auto-generated destructor stub
}

