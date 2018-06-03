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

    Colour colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);

    for (auto w: s.worms) {

        int id = w.first;
        ElementDTO worm_info = w.second;

        int position_worm_x = get_pixels(worm_info.x);
        int position_worm_y = get_pixels(worm_info.y);

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

std::map<Weapon_Name,Animation> GraphicDesigner::create_weapons(){
    std::map<Weapon_Name,Animation> animations;
    Animation bazooka = AnimationFactory::get_bazooka();
    animations.insert(std::pair<Weapon_Name,Animation>(W_Bazooka, bazooka));

    Animation mortar = AnimationFactory::get_mortar();
    animations.insert(std::pair<Weapon_Name,Animation>(Mortar,mortar));

    Animation banana = AnimationFactory::get_banana();
    animations.insert(std::pair<Weapon_Name,Animation>(Banana,banana));

    Animation green_granade = AnimationFactory::get_green_granade();
    animations.insert(std::pair<Weapon_Name,Animation>(Green_Grenade,green_granade));

    Animation red_granade = AnimationFactory::get_red_granade();
    animations.insert(std::pair<Weapon_Name,Animation>(Red_Grenade,red_granade));

    Animation holy_granade = AnimationFactory::get_holy_granade();
    animations.insert(std::pair<Weapon_Name,Animation>(Holy_Grenade,holy_granade));

    Animation air_attack = AnimationFactory::get_air_attack();
    animations.insert(std::pair<Weapon_Name,Animation>(W_Air_Attack,air_attack));

    Animation dynamite = AnimationFactory::get_dynamite();
    animations.insert(std::pair<Weapon_Name,Animation>(Dynamite,dynamite));

    Animation explosion = AnimationFactory::get_explosion();
    animations.insert(std::pair<Weapon_Name,Animation>(Explosion,explosion));
    return animations;
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


    this->worms = create_worms(initial_stage);
    printf(" size = %li\n", this->worms.size() );

    this->weapons = create_weapons();

}


void GraphicDesigner::show_beams(StageDTO s, SDL_Surface *screen){

    Colour colorkey_beam(BEAM_R,BEAM_G,BEAM_B);

    for (auto beam_info: s.beams) {

        //debug_box2d_figure(screen, beam_info);

        int up_left_vertex_x = get_pixels(beam_info.x);
        int up_left_vertex_y = get_pixels(beam_info.y);

        Picture beam(BEAM, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        beam.draw(screen,up_left_vertex_x, up_left_vertex_y);

    }

}

void GraphicDesigner::show_worms(StageDTO s, SDL_Surface *screen){

    for (auto w: s.worms) {

        ElementDTO worm_info = w.second;

        //debug_box2d_figure(screen, worm_info);

        int up_left_vertex_x = get_pixels(worm_info.x);
        int up_left_vertex_y = get_pixels(worm_info.y);

        std::map<int,WormAnimation>::iterator worms_iter = this->worms.find(w.first);
        worms_iter->second.move(up_left_vertex_x, up_left_vertex_y);
        worms_iter->second.show(screen);

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
    this->worms.find(id);
 }


 void GraphicDesigner::show_weapon( StageDTO s,SDL_Surface * screen){
    for (auto w: s.weapons) {

        //debug_box2d_figure(screen, w);

        int up_left_vertex_x = get_pixels(w.x);
        int up_left_vertex_y = get_pixels(w.y);


        std::map<Weapon_Name,Animation>::iterator weapon_iter = this->weapons.find(w.weapon);
        weapon_iter->second.continue_internal_movement();
        weapon_iter->second.draw(screen,up_left_vertex_x, up_left_vertex_y);

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
    Uint32 colorkey = SDL_MapRGBA(screen->format, color.r, color.g, color.b,0.2);
    SDL_FillRect(screen, &rectangle, colorkey);

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
    SDL_BlitSurface(text, &dimention, screen, &position);


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
    SDL_BlitSurface(this->power_bar, &dimention, screen, &position);


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
    SDL_BlitSurface(this->weapons_menu, &dimention, screen, &position);
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
    Uint32 withe  = SDL_MapRGBA(screen->format, 255, 255,255,100);
    SDL_FillRect(screen, &white_rec, withe);

    SDL_Rect rectangle;
    rectangle.x = this->screen_width/2 - 2;
    rectangle.y = 25;
    rectangle.h = text->h;
    rectangle.w = text->w;
    Uint32 colorkey = SDL_MapRGBA(screen->format, 0, 0, 0, 0);
    SDL_FillRect(screen, &rectangle, colorkey);

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
    SDL_BlitSurface(text, &dimention, screen, &position);


}



GraphicDesigner::~GraphicDesigner() {
	// TODO Auto-generated destructor stub
}

