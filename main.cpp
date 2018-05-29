#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "Client_Constants.h"
#include "resources_definitions.h"

#include "stage.h"
#include "Beam.h"
#include "Worm.h"


//g++ -std=c++11 main.cpp stage.cpp Beam.cpp Worm.cpp Constants.cpp Weapon.cpp Bazooka.cpp -lBox2D -lSDL -lSDL_image -lyaml-cpp -lSDL_ttf  -g


using std::cout;
using std::endl;
using std::string;
using std::map;
using std::pair;







/////////////////////////////////////////////////////////////////////////////////////////
enum Color_name{
    White,
    Orange,
    Green,
    Purple,
    Pink,
    Yellow,
    Red,
    Blue
};


class Color{
public:
    int r,g,b;
    Color(int r,int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }

    static Color create(Color_name color_name){
        switch(color_name){
            case(White):{
                Color white(255,255,255);
                return white;
            }
            case(Orange):{
                Color orange(244, 95, 66);
                return orange;
            }
            case(Yellow):{
                Color yellow(229, 220, 57);
                return yellow;
            }
            case(Green):{
                Color green(57, 229, 114);
                return green;
            }
            case Blue:{
                Color blue(83, 144, 237);
                return blue;
            }
            case Purple:{
                Color purple(166, 109, 219);
                return purple;
            }
            case Pink:{
                Color pink(239, 115, 198);
                return pink;
            }
            case Red:{
                Color red(237, 78, 102);
                return red;
            }
        }
        Color black(0,0,0);
        return black;

    }

};

/////////////////////////////////////////////////////////////////////////////////////////



class Picture{
    int rows, columns;
    int row_num, column_num;
    int w, h;

    Direction default_direction;
    SDL_Surface *surface;
    SDL_Surface *flipped;

SDL_Rect get_dimention(){
    SDL_Rect dimention;
    // Separaciones de 2 píxeles dentro de las rejillas para observar
    // bien donde empieza una original y donde termina la otra
    dimention.w = this->w - 2;
    dimention.h = this->h - 2;

    // Cálculo de la posición de la original // dentro de la rejilla

    dimention.x = (this->column_num * this->w) + 2;
    dimention.y = (this->row_num * this->h) + 2;

    return dimention;
}


SDL_Surface * flip(SDL_Surface * original, Uint32 colorkey){
        SDL_Rect origen;
        SDL_Rect destino;

        // Origen -> ancho una línea
        // Comienzo de copia por el principio
        origen.x = 0;
        origen.y = 0;
        origen.w = 1;
        origen.h = original->h;
        // Destino -> ancho una lína
        // Comienzo de 'pegado' por el final
        // Para lograr la inversión
        destino.x = original->w;
        destino.y = 0;
        destino.w = 1;
        destino.h = original->h;
        SDL_Surface *flipped;
        // Pasamos la imagen a formato de pantalla
        flipped = SDL_DisplayFormat(original);
        if(flipped == NULL) {
            cout << "No podemos convertir la imagen al formato de pantalla" << endl;
            return NULL;
        }

        // Preparamos el rectángulo nuevo vacío del color transparente
        SDL_FillRect(flipped, NULL, colorkey);
        // Copiamos linea vertical a linea vertical, inversamente
        for(int i = 0; i < original->w; i++) {
            SDL_BlitSurface(original, &origen, flipped, &destino);
            origen.x = origen.x + 1; destino.x = destino.x - 1;
       }
       return flipped;
}





public:
    Picture(const char * bmp_path, Color color, int columns, int rows){
        this->rows = rows;
        this->columns = columns;
        this->row_num = 0;
        this->column_num = 0;
        this->default_direction = Left; //todas las fotos estan para la izquierda


        SDL_Surface *tmp = IMG_Load(bmp_path);
        //SDL_Surface *tmp = SDL_LoadBMP(bmp_path);
        if (!tmp) {
            cout <<"Couldn't create surface from image:" << bmp_path << SDL_GetError() << endl;
            return;
        }

        this->surface = SDL_DisplayFormat(tmp);
        SDL_FreeSurface(tmp);

        if(this->surface == NULL) {
            cout << "Error: " << SDL_GetError() << endl; exit(1);
        }

        // Calculamos el color transparente, en nuestro caso el verde
         Uint32 colorkey = SDL_MapRGB(this->surface->format, color.r, color.g, color.b);
        // Lo establecemos como color transparente
        SDL_SetColorKey(this->surface, SDL_SRCCOLORKEY, colorkey);

        this->flipped = flip(surface, colorkey);

        // El ancho de una original es el total entre el número de columnas
        this->w = surface->w / columns;
        // El alto de una original es el total entre el número de filas
        this->h = surface->h / rows;
}
int get_height(){
    return this->h;
}

int get_width(){
    return this->w;
}

void draw(SDL_Surface *screen, int x, int y){
    SDL_Rect position;
    position.x = x;
    position.y = y;
    draw(screen, position, this->default_direction);
}



void draw(SDL_Surface *screen, SDL_Rect position, Direction direction){
    SDL_Rect dimention = get_dimention();

    position.h = dimention.h;
    position.w = dimention.w;

    //printf("x = %i, y = %i, h = %i, w = %i \n",position.x, position.y, position.h, position.w );

    if(this->default_direction != direction){
        SDL_BlitSurface(this->flipped, &dimention, screen, &position);
        return;
    }

    SDL_BlitSurface(this->surface, &dimention, screen, &position);

}


void next_sprite_figure(Direction direction){
    if(this->default_direction != direction){ //hay que usar la inversa
        this->column_num -=1; // paso al anterior en la misma fila
        if(this->column_num <= 0){ //si estoy en el primero
            this->column_num = this->columns - 1; //paso al ultimo
            this->row_num += 1; //pero de la fila de abajo
            if(this->row_num >= this->rows){
                this->row_num = 0; //si estaba en el ultimo vuelvo a empezar

            }
        }
        return;
    }

    this->column_num +=1; // paso al siguiente en la misma fila
    if(this->column_num >= this->columns){ //si estoy en el ultimo de la fila
        this->column_num = 0; //paso al primero
        this->row_num += 1; //pero de la fila de abajo
        if(this->row_num >= this->rows){
            this->row_num = 0; //si estaba en el ultimo vuelvo a empezar
        }
    }
}

void previous_sprite_figure(Direction direction){
    if(this->default_direction != direction){ //hay que usar la inversa
        this->column_num +=1; // paso al siguiente en la misma fila
        if(this->column_num >= this->columns){ //si ya no hay mas
            this->column_num = 0; //paso al primero
            this->row_num -= 1; //pero de la fila de arriba
            if(this->row_num < 0){
                this->row_num = this->rows - 1; //si estaba en el ultimo vuelvo a empezar

            }
        }
        return;
    }

    this->column_num -=1; // paso al anterior en la misma fila
    if(this->column_num < 0){ //si ya no hay anterior
        this->column_num = this->columns - 1; //paso al ultomo
        this->row_num -= 1; //pero de la fila de arriba
        if(this->row_num < 0){
            this->row_num = this->rows - 1; //si estaba en el primero vuelvo al ultimo
        }
    }
}

bool is_in_first_figure(Direction direction){
    if(this->default_direction != direction){
        return (this->row_num <= 0 && this->column_num >= this->columns-1);
    }
    return (this->row_num <= 0 && this->column_num <= 0);

}

bool is_in_last_figure(Direction direction){
    if(this->default_direction != direction){
        return (this->row_num >= this->rows-1 && this->column_num <= 0);
    }
    return (this->row_num >= this->rows-1 && this->column_num >= this->columns-1);

}


/*~Picture(){
    SDL_FreeSurface(this->surface);
}*/

};


/////////////////////////////////////////////////////////////////////////////////////////

class Animation {

    Picture picture;
    SDL_Rect dimention;
    int figures_num;
    int step;
    Direction direction;


public:

    Animation(const char * bmp_path, Color color,int columns, int rows):
    picture(bmp_path, color, columns, rows ){
        this->figures_num = columns * rows;
        this->step = 0;
        this->direction = Left;

    }

    int get_height(){
        return this->picture.get_height();
    }

    int get_width(){
        return this->picture.get_width();
    }


    void set_current_direction(Direction direction){
        this->direction = direction;
    }

    Direction get_current_direction(){
        return this->direction;
    }


    void draw(SDL_Surface *screen, int x, int y){
        SDL_Rect position;
        position.x = x;
        position.y = y;
        this->picture.draw(screen, position, this->direction);
    }

    bool continue_internal_movement(){
        this->picture.next_sprite_figure(this->direction);
        this->step += 1;
        if(this->step == this->figures_num){
            this->step = 0;
            return false;
        }
        return true;
    }

    bool point_up(){
        if(!this->picture.is_in_last_figure(this->direction)){
            this->picture.next_sprite_figure(this->direction);
            return true;
        }
        return false;

    }
    bool point_down(){
        if(!this->picture.is_in_first_figure(this->direction)){
            this->picture.previous_sprite_figure(this->direction);
            return true;
        }
        return false;
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

    static Animation get_worm_missile(){
        Color colorkey(WORM_MISSILE_R,WORM_MISSILE_G,WORM_MISSILE_B);
        Animation worm(WORM_MISSILE,colorkey,WORM_MISSILE_COLUMNS,WORM_MISSILE_ROWS);
        return worm;
    }

    static Animation get_worm_bat(){
        Color colorkey(WORM_BAT_R,WORM_BAT_G,WORM_BAT_B);
        Animation worm(WORM_BAT,colorkey,WORM_BAT_COLUMNS,WORM_BAT_ROWS);
        return worm;
    }

    static Animation get_worm_banana(){
        Color colorkey(WORM_BANANA_R,WORM_BANANA_G,WORM_BANANA_B);
        Animation worm(WORM_BANANA,colorkey,WORM_BANANA_COLUMNS,WORM_MISSILE_ROWS);
        return worm;
    }

    static Animation get_worm_green_granade(){
        Color colorkey(WORM_GREEN_GRANADE_R,WORM_GREEN_GRANADE_G,WORM_GREEN_GRANADE_B);
        Animation worm(WORM_GREEN_GRANADE,colorkey,WORM_GREEN_GRANADE_COLUMNS,WORM_GREEN_GRANADE_ROWS);
        return worm;
    }

    static Animation get_worm_red_granade(){
        Color colorkey(WORM_RED_GRANADE_R,WORM_RED_GRANADE_G,WORM_RED_GRANADE_B);
        Animation worm(WORM_RED_GRANADE,colorkey,WORM_RED_GRANADE_COLUMNS,WORM_RED_GRANADE_ROWS);
        return worm;
    }

    static Animation get_worm_holy_granade(){
        Color colorkey(WORM_HOLY_GRANADE_R,WORM_HOLY_GRANADE_G,WORM_HOLY_GRANADE_B);
        Animation worm(WORM_HOLY_GRANADE,colorkey,WORM_HOLY_GRANADE_COLUMNS,WORM_HOLY_GRANADE_ROWS);
        return worm;
    }

    static Animation get_worm_teletrans(){
        Color colorkey(WORM_TELETRANS_R,WORM_TELETRANS_G,WORM_TELETRANS_B);
        Animation worm(WORM_TELETRANS,colorkey,WORM_TELETRANS_COLUMNS,WORM_TELETRANS_ROWS);
        return worm;
    }

    static Animation get_worm_air_attack(){
        Color colorkey(WORM_AIR_ATTACK_R,WORM_AIR_ATTACK_G,WORM_AIR_ATTACK_B);
        Animation worm(WORM_AIR_ATTACK,colorkey,WORM_AIR_ATTACK_COLUMNS,WORM_AIR_ATTACK_ROWS);
        return worm;
    }

    static Animation get_worm_dynamite(){
        Color colorkey(WORM_DYNAMITE_R,WORM_DYNAMITE_G,WORM_DYNAMITE_B);
        Animation worm(WORM_DYNAMITE,colorkey,WORM_DYNAMITE_COLUMNS,WORM_DYNAMITE_ROWS);
        return worm;
    }

    //-----------------------Armas --------------------------------------------------


     static Animation get_bazooka(){
        Color colorkey(BAZOOKA_R,BAZOOKA_G,BAZOOKA_B);
        Animation worm(BAZOOKA,colorkey,BAZOOKA_COLUMNS,BAZOOKA_ROWS);
        return worm;
    }

       static Animation get_mortar(){
        Color colorkey(MORTAR_R,MORTAR_G,MORTAR_B);
        Animation worm(MORTAR,colorkey,MORTAR_COLUMNS,MORTAR_ROWS);
        return worm;
    }

     static Animation get_banana(){
        Color colorkey(BANANA_R,BANANA_G,BANANA_B);
        Animation worm(BANANA,colorkey,BANANA_COLUMNS,BANANA_ROWS);
        return worm;
    }

    static Animation get_green_granade(){
        Color colorkey(GREEN_GRANADE_R,GREEN_GRANADE_G,GREEN_GRANADE_B);
        Animation worm(GREEN_GRANADE,colorkey,GREEN_GRANADE_COLUMNS,GREEN_GRANADE_ROWS);
        return worm;
    }

    static Animation get_red_granade(){
        Color colorkey(RED_GRANADE_R,RED_GRANADE_G,RED_GRANADE_B);
        Animation worm(RED_GRANADE,colorkey,RED_GRANADE_COLUMNS,RED_GRANADE_ROWS);
        return worm;
    }

    static Animation get_holy_granade(){
        Color colorkey(HOLY_GRANADE_R,HOLY_GRANADE_G,HOLY_GRANADE_B);
        Animation worm(HOLY_GRANADE,colorkey,HOLY_GRANADE_COLUMNS,HOLY_GRANADE_ROWS);
        return worm;
    }


    static Animation get_air_attack(){
        Color colorkey(AIR_ATTACK_R,AIR_ATTACK_G,AIR_ATTACK_B);
        Animation worm(AIR_ATTACK,colorkey,AIR_ATTACK_COLUMNS,AIR_ATTACK_ROWS);
        return worm;
    }

    static Animation get_dynamite(){
        Color colorkey(DYNAMITE_R,DYNAMITE_G,DYNAMITE_B);
        Animation worm(DYNAMITE,colorkey,DYNAMITE_COLUMNS,DYNAMITE_ROWS);
        return worm;
    }

     static Animation get_explosion(){
        Color colorkey(EXPLOSION_R,EXPLOSION_G,EXPLOSION_B);
        Animation worm(EXPLOSION,colorkey,EXPLOSION_COLUMNS,EXPLOSION_ROWS);
        return worm;
    }

};

/////////////////////////////////////////////////////////////////////////////////////////

enum State{
   Still,
   Walk,
   Fall,
   Jump_state,
   Worm_missile,
   Worm_banana,
   Worm_bat,
   Worm_green_granade,
   Worm_red_granade,
   Worm_holy_granade,
   Worm_teletrans,
   Worm_air_attack,
   Worm_dynamite
};
//////////////////////////////////////////////////////


static  std::map<Weapon_Name, State> weapons_states {
    {W_Air_Attack, Worm_air_attack},
    {W_Bazooka,Worm_missile},
    {Dynamite,Worm_dynamite},
    {Mortar,Worm_missile},
    {Green_Granade,Worm_green_granade},
    {Holy_Granade,Worm_holy_granade},
    {Red_Granade,Worm_red_granade},
    {Teletrans,Worm_teletrans},
    {Banana,Worm_banana},
    {Baseboll_Bat,Worm_bat}
};


//armas con mira
static const std::vector<State> weapons_states_with_degrees(
    {Worm_missile,
    Worm_green_granade,
    Worm_holy_granade,
    Worm_red_granade,
    Worm_banana,
    Worm_bat}
);

//armas con intensidad
static const std::vector<State> weapons_states_with_power(
    {Worm_missile,
    Worm_green_granade,
    Worm_holy_granade,
    Worm_red_granade,
    Worm_banana}
);


/////////////////////////////////////////////////////////////////

#define GRADES_PER_STEP 5.8064516129 //180/31

class Worm_Animation_Controller{
    int x, y;
    Direction direction;
    State state;
    float degrees;
    int weapon_power;
    int timer;
    std::map<int,Animation> animations;

public:

Worm_Animation_Controller(int initial_x, int initial_y, Direction initial_dir){

    Animation worm_walk = Animation_Factory::get_worm_walk();
    this->animations.insert(std::pair<int,Animation>(Still,worm_walk));
    this->animations.insert(std::pair<int,Animation>(Walk,worm_walk));

    Animation worm_fall = Animation_Factory::get_worm_fall();
    this->animations.insert(std::pair<int,Animation>(Fall,worm_fall));

    Animation worm_jump = Animation_Factory::get_worm_jump();
    this->animations.insert(std::pair<int,Animation>(Jump_state,worm_jump));

    Animation worm_missile = Animation_Factory::get_worm_missile();
    this->animations.insert(std::pair<int,Animation>(Worm_missile,worm_missile));

    Animation worm_banana = Animation_Factory::get_worm_banana();
    this->animations.insert(std::pair<int,Animation>(Worm_banana,worm_banana));

    Animation worm_bat = Animation_Factory::get_worm_bat();
    this->animations.insert(std::pair<int,Animation>(Worm_bat,worm_bat));

    Animation worm_green_granade = Animation_Factory::get_worm_green_granade();
    this->animations.insert(std::pair<int,Animation>(Worm_green_granade,worm_green_granade));

    Animation worm_red_granade = Animation_Factory::get_worm_red_granade();
    this->animations.insert(std::pair<int,Animation>(Worm_red_granade,worm_red_granade));

    Animation worm_holy_granade = Animation_Factory::get_worm_holy_granade();
    this->animations.insert(std::pair<int,Animation>(Worm_holy_granade,worm_holy_granade));

    Animation worm_teletrans = Animation_Factory::get_worm_teletrans();
    this->animations.insert(std::pair<int,Animation>(Worm_teletrans,worm_teletrans));

    Animation worm_air_attack = Animation_Factory::get_worm_air_attack();
    this->animations.insert(std::pair<int,Animation>(Worm_air_attack,worm_air_attack));

    Animation worm_dynamite = Animation_Factory::get_worm_dynamite();
    this->animations.insert(std::pair<int,Animation>(Worm_dynamite,worm_dynamite));

    this->x = initial_x;
    this-> y = initial_y;
    this->state = Still;
    this->direction = initial_dir;
    for(std::map<int,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){
        animation_iter->second.set_current_direction(initial_dir);
    }
    this->weapon_power = 0;
    this->timer = 5;
}

bool has_weapon(){
    return (this->state != Walk && this->state != Still && this->state != Jump_state && this->state != Fall);
}

bool has_weapon_to_click(){
    return (this->state == Worm_teletrans || this->state == Worm_air_attack);
}

bool has_point_weapon(){ //armas con las que no se puede apuntar
    return std::find(weapons_states_with_degrees.begin(), weapons_states_with_degrees.end(), this->state) //
    != weapons_states_with_degrees.end();
}

void change_direction(Direction direction){
    for(std::map<int,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){
        animation_iter->second.set_current_direction(direction);
    }
    this->direction = direction;
    if(this->state == Still){
        change_state(Walk);
    }

}

void change_state(State state){
    this->state = state; //sigue en la misma dire que antes
}

void take_weapon(Weapon_Name weapon){
    this->timer = 5;
    this->degrees = -90;
    std::map<Weapon_Name,State>::iterator weapon_state = weapons_states.find(weapon);
    change_state(weapon_state->second);
    if( std::find(weapons_states_with_power.begin(), weapons_states_with_power.end(), this->state) //
    != weapons_states_with_power.end() && this->weapon_power == 0){
        this->weapon_power = 10;
    }else{
        this->weapon_power = 0;
    }
        
    


}

float point_down_weapon(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    if(animation_iter->second.point_down()){
        this->degrees-=GRADES_PER_STEP; //31 fotos/180 grados
        
    }
    return this->degrees;

}

float point_up_weapon(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    if(animation_iter->second.point_up()){
            this->degrees+=GRADES_PER_STEP; //31 fotos/180 grados
    }
    return this->degrees;
}

float get_degrees(){
    return this->degrees;
}

bool add_power(){
    if(this->weapon_power < 100){
        this->weapon_power +=1;
        return true;
    }
    return false;
}
int get_weapon_power(){
    return this->weapon_power;
}

void set_timer(int timer){
    this->timer = timer;
}

int get_timer(){
    return this->timer;
}



Direction get_direction(){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    return animation_iter->second.get_current_direction();
}


void move(int position_x, int position_y){
    if(this->x = position_x && this->y == y && this->state == Fall){ // se cayo sobre una viga
        this->state = Still;
    }
    if(this->state != Jump_state && position_y > this->y){ //aumenta el y, se cae
        this->state = Fall;
    }
    this->x = position_x;
    this->y = position_y;
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);

    if(this->state == Jump_state || this->state ==Walk){
        if(!animation_iter->second.continue_internal_movement()){
            this->state = Still;
        }
    }

}

int get_x(){
    return this->x;
}
int get_y(){
    return this->y;
}

void show(SDL_Surface * screen){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    animation_iter->second.draw(screen, this->x, this->y);
}

};

/////////////////////////////////////////////////////////////////////////////////////////////





class Graphic_Designer{
    SDL_Surface * screen;
    int screen_height;
    int screen_width;
    TTF_Font *font;
    TTF_Font *time_font;
    SDL_Surface *power_bar;
    SDL_Surface * weapons_menu;




public:

Graphic_Designer(SDL_Surface * screen, int screen_height, int screen_width){
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





}



void show_life(int life, int x, int y, Color color){

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

void show_powerbar(int power){
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

void show_weapons_menu(int size){
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

bool is_inside_weapon_menu(int x, int y){
    int x_relative = x -(this->screen_width - this->weapons_menu->w -5);
    int y_relative = y - 5;
    if(x_relative < 0 || y_relative > this->weapons_menu->h){
        return false;
    }
    return true;
}


Weapon_Name choose_weapon(int x, int y){

    int icon_y = this->weapons_menu->h/10;
    if(y < icon_y){
        return W_Air_Attack;

    }
    if(y < icon_y*2){
        return Baseboll_Bat;
    }
    if(y < icon_y*3){
        return W_Bazooka;
    }
    if(y < icon_y*4){
        return Red_Granade;
    }
    if(y < icon_y*5){
        return Green_Granade;
    }
    if(y < icon_y*6){
        return Holy_Granade;

    }
    if(y < icon_y*7){
        return Mortar;

    }
    if(y < icon_y*8){
        return Teletrans;
    }
    if(y < icon_y*9){
        return Dynamite;
    }
    return Banana;
}

void show_timer(int second){
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


};


//////////////////////////////////////////////////////////////////////////////////////////


float get_pixels(float meter_position){
    return  23.5*meter_position;
}

void debug_box2d_figure(SDL_Surface *screen, ElementDTO element_info){

    //dibujo un rectangulo
    SDL_Rect rectangle;
    rectangle.x = get_pixels(element_info.x);
    rectangle.y = get_pixels(element_info.y);
    rectangle.h = get_pixels(element_info.h);
    rectangle.w = get_pixels(element_info.w);

    Uint32 colorkey = SDL_MapRGBA(screen->format, 0, 255, 0,0.5);
    SDL_FillRect(screen, &rectangle, colorkey);

}




////////////////////////////////////////////////////////////////////////////////////////////7/////////


class Event_Controller{
    Graphic_Designer graphic_designer;
    int screen_height;
    int screen_width;
    Stage & stage;
    SDL_Event &  event;
    bool wait_for_destination_clicl;
    bool wait_for_weapon_click;
    ActionDTO action;

float meters_conversor(int pixel){
    return (pixel+0.0)/23.5;

}
void air_attack(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(W_Air_Attack);
    this->wait_for_destination_clicl = true;
    this->action.type = Take_weapon;
    this->action.weapon = W_Air_Attack;
    this->stage.make_action(this->action);
}

void bazooka(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(W_Bazooka);
    this->action.type = Take_weapon;
    this->action.weapon = W_Bazooka;
    this->stage.make_action(this->action);
}

void dynamite(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(Dynamite);
    this->action.type = Take_weapon;
    this->action.weapon = Dynamite;
    this->stage.make_action(this->action);
}

void green_granade(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(Green_Granade);
    this->action.type = Take_weapon;
    this->action.weapon = Green_Granade;
    this->stage.make_action(this->action);
}

void holy_granade(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(Holy_Granade);
    this->action.type = Take_weapon;
    this->action.weapon = Holy_Granade;
    this->stage.make_action(this->action);
}

void mortar(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(Mortar);
    this->action.type = Take_weapon;
    this->action.weapon = Mortar;
    this->stage.make_action(this->action);
}

void red_granade(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(Red_Granade);
    this->action.type = Take_weapon;
    this->action.weapon = Red_Granade;
    this->stage.make_action(this->action);
}

void teletrans(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(Teletrans);
    this->action.type = Take_weapon;
    this->action.weapon = Teletrans;
    this->stage.make_action(this->action);
}

void banana(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(Banana);
    this->action.type = Take_weapon;
    this->action.weapon = Banana;
    this->stage.make_action(this->action);
}

void baseboll_bat(Worm_Animation_Controller& turn_worm){
    turn_worm.take_weapon(Baseboll_Bat);
    this->action.type = Take_weapon;
    this->action.weapon = Baseboll_Bat;
    this->stage.make_action(this->action);
}

void shot(Worm_Animation_Controller& turn_worm,int x, int y){
    if(turn_worm.has_weapon()){
        printf("%f %f\n", meters_conversor(x),meters_conversor(y));
        this->action.type = Shot_weapon;
        this->action.x = meters_conversor(x);
        this->action.y = meters_conversor(y);
        this->action.weapon_degrees = turn_worm.get_degrees();
        this->action.direction = turn_worm.get_direction();
        this->action.power = turn_worm.get_weapon_power();
        this->action.time_to_explode = turn_worm.get_timer();
        this->wait_for_destination_clicl = true;
        this->stage.make_action(this->action);
    }
    else{
        cout << "no tiene arma para disparar" << endl;
    }
}

void click(Worm_Animation_Controller& turn_worm){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(turn_worm.has_weapon_to_click()){
        shot(turn_worm,x,y);
    }

    else if(this->wait_for_weapon_click){
        if(graphic_designer.is_inside_weapon_menu(x,y)){
            Weapon_Name  weapon = graphic_designer.choose_weapon(x,y);
            turn_worm.take_weapon(weapon);
            this->action.type = Take_weapon;
            this->action.weapon = weapon;
            this->stage.make_action(this->action);
        }
    }
    this->wait_for_weapon_click = false;

}

void up(Worm_Animation_Controller& turn_worm){
    if(turn_worm.has_point_weapon()){
        float degrees = turn_worm.point_up_weapon();
        printf("%f\n",degrees );
    }else{
        turn_worm.change_state(Jump_state);
        this->action.type = Make_move;
        this->action.move = Jump;
        this->action.direction = turn_worm.get_direction();
        this->stage.make_action(this->action);
    }

}

void down(Worm_Animation_Controller& turn_worm){
    if(turn_worm.has_point_weapon()){
        float degrees = turn_worm.point_down_weapon();
        printf("%f\n",degrees );
    }

}

void right(Worm_Animation_Controller& turn_worm){
    turn_worm.change_direction(Right);
    this->action.type = Make_move;
    this->action.move = Walk_right;
    this->stage.make_action(this->action);

}

void left(Worm_Animation_Controller& turn_worm){
    turn_worm.change_direction(Left);
    this->action.type = Make_move;
    this->action.move = Walk_left;
    this->stage.make_action(this->action);
}


void space(Worm_Animation_Controller& turn_worm){
    int power = turn_worm.get_weapon_power();
    if(power >0 && power < 100){
        turn_worm.add_power();
    }
}




void mouse_motion(){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x > screen_width - 5 && y < 5){
        cout << "Se quiere elegir un arma:" << endl;
        printf("%i %i\n",x,y);
        this->wait_for_weapon_click = true;
    }
}

void weapon_shot(Worm_Animation_Controller& turn_worm){
    shot(turn_worm,turn_worm.get_x(),turn_worm.get_y());

}






public:
Event_Controller(SDL_Event & event, Stage & stage, int screen_height, int screen_width, Graphic_Designer & graphic_designer):
        event(event),
        stage(stage),
        graphic_designer(graphic_designer){
            this->screen_height = screen_height;
            this-> screen_width = screen_width;
            this->wait_for_destination_clicl = false;
            this->wait_for_weapon_click = false;
            this->action.worm_id = 0;

}



bool continue_running(Worm_Animation_Controller& turn_worm){
    if(this->wait_for_weapon_click){
        this->graphic_designer.show_weapons_menu(100);
    }

    if(SDL_PollEvent(&this->event) != 1){
        return true; // no hay nuevos event
    }
    switch(event.type){
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONUP:
            click(turn_worm);
            break;
        case SDL_MOUSEMOTION:
            mouse_motion();
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    return false;
                case SDLK_w:
                    cout << "disparo" << endl;
                    weapon_shot(turn_worm);
                    break;
                case SDLK_LEFT:
                    left( turn_worm);
                    break;
                case SDLK_RIGHT:
                    right( turn_worm);
                    break;
                case SDLK_UP:
                    up( turn_worm);
                    break;
                case SDLK_DOWN:
                    down( turn_worm);
                    break;
                //-----------potencia..................
                case SDLK_SPACE:
                    space( turn_worm);
                    break;
                case SDLK_0:
                    turn_worm.set_timer(0);
                    break;
                case SDLK_1:
                    turn_worm.set_timer(1);
                    break;
                case SDLK_2:
                    turn_worm.set_timer(2);
                    break;
                case SDLK_3:
                    turn_worm.set_timer(3);
                    break;
                case SDLK_4:
                    turn_worm.set_timer(4);
                    break;
                case SDLK_5:
                    turn_worm.set_timer(5);
                    break;
                case SDLK_6:
                    turn_worm.set_timer(6);
                    break;
                case SDLK_7:
                    turn_worm.set_timer(7);
                    break;
                case SDLK_8:
                    turn_worm.set_timer(8);
                    break;
                case SDLK_9:
                    turn_worm.set_timer(9);
                    break;
                //-------------ARMAS--------------------
                case SDLK_a:
                    air_attack( turn_worm);
                    break;
                case SDLK_b:
                    bazooka( turn_worm);
                    break;
                case SDLK_d:
                    dynamite( turn_worm);
                    break;
                case SDLK_g:
                    green_granade( turn_worm);
                    break;
                case SDLK_h:
                    holy_granade( turn_worm);
                    break;
                case SDLK_m:
                    mortar( turn_worm);
                    break;
                case SDLK_r:
                    red_granade( turn_worm);
                    break;
                case SDLK_t:
                    teletrans( turn_worm);
                    break;
                case SDLK_u:
                    banana( turn_worm);
                    break;
                case SDLK_v:
                    baseboll_bat( turn_worm);
                    break;
                }
                break;
    }
    return true;
    }

};




////////////////////////////////////////////////////////////////////////////////////////////////////

//armas con timer
static const std::vector<Weapon_Name> weapons_with_timer(
    {Holy_Granade,
    Dynamite,
    Baseboll_Bat,
    Red_Granade,
    Banana}
);



class Weapons_Animation_Controller{
    std::map<Weapon_Name,Animation> animations;

public:

Weapons_Animation_Controller(int i){

        Animation bazooka = Animation_Factory::get_bazooka();
        this->animations.insert(std::pair<Weapon_Name,Animation>(W_Bazooka, bazooka));

        Animation mortar = Animation_Factory::get_mortar();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Mortar,mortar));

        Animation banana = Animation_Factory::get_banana();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Banana,banana));

        Animation green_granade = Animation_Factory::get_green_granade();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Green_Granade,green_granade));

        Animation red_granade = Animation_Factory::get_red_granade();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Red_Granade,red_granade));

        Animation holy_granade = Animation_Factory::get_holy_granade();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Holy_Granade,holy_granade));

        Animation air_attack = Animation_Factory::get_air_attack();
        this->animations.insert(std::pair<Weapon_Name,Animation>(W_Air_Attack,air_attack));

        Animation dynamite = Animation_Factory::get_dynamite();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Dynamite,dynamite));

        Animation explosion = Animation_Factory::get_explosion();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Explosion,explosion));
}


bool is_timer_weapon(Weapon_Name weapon){
    return std::find(weapons_with_timer.begin(), weapons_with_timer.end(), weapon) != weapons_with_timer.end();
}

void show_weapon( StageDTO s,SDL_Surface * screen, Graphic_Designer & gd){
    for (auto w: s.weapons) {

        debug_box2d_figure(screen, w);

        int up_left_vertex_x = get_pixels(w.x);
        int up_left_vertex_y = get_pixels(w.y);


        std::map<Weapon_Name,Animation>::iterator weapon_iter = animations.find(w.weapon);
        weapon_iter->second.continue_internal_movement();
        weapon_iter->second.draw(screen,up_left_vertex_x, up_left_vertex_y);

        if(is_timer_weapon(w.weapon)){
            gd.show_timer(w.timer);
        }
        
    }
}

};
//////////////////////////////////////////////////////////////////////////////


void show_beams(StageDTO s, SDL_Surface *screen){

    Color colorkey_beam(BEAM_R,BEAM_G,BEAM_B);

    for (auto beam_info: s.beams) {

        debug_box2d_figure(screen, beam_info);

        int up_left_vertex_x = get_pixels(beam_info.x);
        int up_left_vertex_y = get_pixels(beam_info.y);

        Picture beam(BEAM, colorkey_beam,BEAM_COLUMNS,BEAM_ROWS);
        beam.draw(screen,up_left_vertex_x, up_left_vertex_y);

    }

}
/////////////////////////////////////////////////////////////////////////////

class Water_animation_controller{
    Color colorkey;
    Animation water_left;
    Animation water_right;
    int screen_height;
    int show_counter;
    int speed;

public:
    Water_animation_controller(int screen_height,int speed):
        colorkey(WATER_R,WATER_G,WATER_B),
        water_left(WATER, this->colorkey,WATER_COLUMNS,WATER_ROWS),
        water_right(WATER, this->colorkey,WATER_COLUMNS,WATER_ROWS)
    {
        this->speed = speed;
        this->show_counter = 0;
        this->screen_height = screen_height - 50;
        this->water_right = water_right;
        this->water_left = water_left;

    }

    void show(SDL_Surface * screen){
        if(this->show_counter == this->speed){
            this->show_counter = 0;
            water_left.continue_internal_movement();
            water_right.continue_internal_movement();

        }else{
            this->show_counter +=1;
        }
        water_left.draw(screen,0,this->screen_height-water_left.get_height());
        water_right.draw(screen,water_left.get_width()-2,this->screen_height-water_right.get_height());

    }


};



//////////////////////////////////////////////////////////////////////////////////

std::map<int,Worm_Animation_Controller> create_worms(StageDTO s, SDL_Surface *screen){

    printf("creador de gusanos\n");

    std::map<int,Worm_Animation_Controller> worms;

    Color colorkey(WORM_WALK_R,WORM_WALK_G,WORM_WALK_B);

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
        Worm_Animation_Controller worm(position_worm_x, position_worm_y, dir);
        worms.insert(std::pair<int,Worm_Animation_Controller>(id,worm));
        worm.show(screen);

    }

    return worms;
}


static  std::vector<Color_name> possible_colors {
    Orange,
    Green,
    Purple,
    Pink,
    Yellow
};


void show_worms(StageDTO s, SDL_Surface *screen, std::map<int,Worm_Animation_Controller> & worms, Graphic_Designer & graphic_designer){

    for (auto w: s.worms) {

        ElementDTO worm_info = w.second;

        debug_box2d_figure(screen, worm_info);

        int up_left_vertex_x = get_pixels(worm_info.x);
        int up_left_vertex_y = get_pixels(worm_info.y);

        std::map<int,Worm_Animation_Controller>::iterator worms_iter = worms.find(w.first);
        worms_iter->second.move(up_left_vertex_x, up_left_vertex_y);
        worms_iter->second.show(screen);

        if(worm_info.player_id > 4){
            cout << "Error: juego no preparado para mas de 4 jugadores" << endl;
        }

        Color player_color = Color::create(possible_colors.at(worm_info.player_id));

        graphic_designer.show_life(worm_info.life,up_left_vertex_x+20,up_left_vertex_y-5,player_color);
        int weapon_power = worms_iter->second.get_weapon_power();
        graphic_designer.show_powerbar(weapon_power);

    }
}



#define SCREEN_DEFAULT_WITH 1366
#define SCREEN_DEFAULT_HIGH 768

int main(int argc, char *args[]){

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    atexit(SDL_Quit);
    int screen_width = SCREEN_DEFAULT_WITH;
    int screen_height = SCREEN_DEFAULT_HIGH;

    const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
    screen_width = info->current_w;
    screen_height = info->current_h;



    if(SDL_VideoModeOK(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF) == 0) {
        // Comprobamos que sea compatible el modo de video
       cout << "Modo video no soportado: " << SDL_GetError() << endl;
       exit(1);
    }

    // Establecemos el modo de video
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(screen_width, screen_height, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(screen == NULL) {
        cout << "No se pudo establecer el modo de video: "
        << SDL_GetError() << endl;
        exit(1);
    }

    // Set the title bar
    SDL_WM_SetCaption("Worms game", "Worms");

    Graphic_Designer graphic_designer(screen, screen_height,screen_width);


    //------------------------------------

    Stage stage("stage1");

    StageDTO s = stage.get_stageDTO();

    Water_animation_controller water(screen_height, 3);

    //dibujo los gusanos en su posicion inicial
    std::map<int,Worm_Animation_Controller> worms = create_worms(s, screen);
    printf(" size = %li\n", worms.size() );

    Weapons_Animation_Controller weapons_controller = Weapons_Animation_Controller(1);


    //------------------------------------


    //turno harcodeado
    std::map<int,Worm_Animation_Controller>::iterator turn_worm_iter = worms.find(0);


    SDL_Event event;
    Event_Controller event_controller(event, stage, screen_height, screen_width, graphic_designer);

    //para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;

    bool running=true;
    while(running ){

        running = event_controller.continue_running(turn_worm_iter->second);
        //actualiza el dibujo de la superficie en la pantalla
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();
        //update
        stage.update();
        StageDTO s = stage.get_stageDTO();

        if((t1 -t0) > 100) {
            // Nueva referencia de tiempo
            t0 = SDL_GetTicks();

            //borro todo lo que estaba
            //toda la pantalla en negro
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
            //dibujo las vigas y el agua
            water.show(screen);
            show_beams(s, screen);

            //dibujo los gusanos
            show_worms(s, screen, worms, graphic_designer);
            weapons_controller.show_weapon(s, screen, graphic_designer);

        }

    }

    return 0;
}
