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
enum Direction{
    Right,
    Left
};


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
    
    {Air_Attack, Worm_air_attack},
    {Bazooka,Worm_missile},
    {Dynamite,Worm_dynamite},
    {Mortar,Worm_missile},
    {Green_Granade,Worm_green_granade},
    {Holy_Granade,Worm_holy_granade},
    {Red_Granade,Worm_red_granade},
    {Teletrans,Worm_teletrans},
    {Banana,Worm_banana},
    {Baseboll_Bat,Worm_bat}
};


static const std::vector<State> weapons_states_to_point(
    {Worm_missile,
    Worm_green_granade,
    Worm_holy_granade,
    Worm_red_granade,
    Worm_teletrans,
    Worm_banana,
    Worm_bat}
);



/////////////////////////////////////////////////////////////////

#define GRADES_PER_STEP 5.8064516129 //180/31

class Worm_Animation_Controller{
    int x, y;
    State state;
    float degrees;
    int weapon_power;
    std::map<int,Animation> animations;
    
public:

Worm_Animation_Controller(int initial_x, int initial_y){
    this->x = initial_x;
    this-> y = initial_y;
    this->state = Still;
    this->degrees = -90;
    this->weapon_power = 0;
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
}


bool has_point_weapon(){ //armas con las que no se puede apuntar
    return std::find(weapons_states_to_point.begin(), weapons_states_to_point.end(), this->state) //
    != weapons_states_to_point.end();
}

void change_direction(Direction direction){
    for(std::map<int,Animation>::iterator animation_iter = this->animations.begin();
        animation_iter != this->animations.end();
        animation_iter ++){ 
        animation_iter->second.set_current_direction(direction);
    }
    if(this->state == Still){
        change_state(Walk);
    }
    
}

void change_state(State state){
    this->state = state; //sigue en la misma dire que antes
}

void take_weapon(Weapon_Name weapon){
    this->degrees = -90;
    std::map<Weapon_Name,State>::iterator weapon_state = weapons_states.find(weapon);
    change_state(weapon_state->second);

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
       this->degrees+=GRADES_PER_STEP;
    }
    return this->degrees;
}

bool add_power(){
    if(this->weapon_power < 100){
        this->weapon_power +=1;
    }
}
int get_weapon_power(){
    return this->weapon_power;
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

void show(SDL_Surface * screen){
    std::map<int,Animation>::iterator animation_iter = animations.find(this->state);
    animation_iter->second.draw(screen, this->x, this->y);
}

};



////////////////////////////////////////////////////////////////////////////////////////////7/////////


class Event_Controller{
    Stage & stage;
    SDL_Event &  event;
    bool wait_for_destination_clicl;
    bool wait_for_potentia;
    ActionDTO action;
public:
Event_Controller(SDL_Event & event, Stage & stage):
        event(event),
        stage(stage){
            this->wait_for_destination_clicl = false;
            this->wait_for_potentia = false;
            this->action.worm_id = 0;

}

float meters_conversor(int pixel){
    return (pixel+0.0)/23.5;

}


bool continue_running(Worm_Animation_Controller& turn_worm){
    SDL_PollEvent(&this->event);
    switch(event.type){
        case SDL_QUIT:
            cout << "se apreto x -> fin" << endl;
            return false;
        case SDL_MOUSEBUTTONUP:
            cout <<"se apreto en mouse"<< endl;
            if(wait_for_destination_clicl){
                int x, y;
                SDL_GetMouseState(&x, &y);
                printf("%f %f\n", meters_conversor(x),meters_conversor(y) );
                this->action.type = Shot_weapon;
                this->action.x = meters_conversor(x);
                this->action.y = meters_conversor(y);
                this->action.power = turn_worm.get_weapon_power();
                this->wait_for_destination_clicl = true;
                this->stage.make_action(this->action);
            }
            break;
            

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    return false;

                case SDLK_LEFT:
                    cout << "se apreto izquierda " << endl;
                    turn_worm.change_direction(Left);
                    this->action.type = Make_move;
                    this->action.move = Walk_left;
                    this->stage.make_action(this->action);    
                    break;
                case SDLK_RIGHT:
                    cout << "se apreto derecha " << endl;
                    turn_worm.change_direction(Right);
                    this->action.type = Make_move;
                    this->action.move = Walk_right;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_UP:
                    cout << "se apreto arriba " << endl;
                    if(turn_worm.has_point_weapon()){
                        float degrees = turn_worm.point_up_weapon();
                        printf("%f\n",degrees );
                    }else{
                        turn_worm.change_state(Jump_state);
                        this->action.type = Make_move;
                        this->action.move = Jump;
                        this->stage.make_action(this->action);
                    }
                    break;
                case SDLK_DOWN:
                    cout << "se apreto abajo " << endl;
                    if(turn_worm.has_point_weapon()){
                        float degrees = turn_worm.point_down_weapon();
                        printf("%f\n",degrees );
                    }
                    break;
                //-----------potencia..................
                case SDLK_SPACE:
                    printf("se apreto espacioo\n");
                    if(this->wait_for_potentia){
                        if(!turn_worm.add_power()){
                            this->wait_for_potentia = false;
                        }
                    }
                    break;
                //-------------ARMAS--------------------
                case SDLK_a:
                    cout << "se apreto a -> Air_attack" << endl;
                    turn_worm.take_weapon(Air_Attack);
                    this->wait_for_destination_clicl = true;
                    this->action.type = Take_weapon;
                    this->action.weapon = Air_Attack;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_b:
                    cout << "se apreto b -> bazooka" << endl;
                    turn_worm.take_weapon(Bazooka);
                    this->action.type = Take_weapon;
                    this->action.weapon = Bazooka;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_d:
                    cout << "se apreto d -> Dynamite" << endl;
                    turn_worm.take_weapon(Dynamite);
                    this->action.type = Take_weapon;
                    this->action.weapon = Dynamite;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_g:
                    cout << "se apreto g -> Green_Granade" << endl;
                    turn_worm.take_weapon(Green_Granade);
                    this->action.type = Take_weapon;
                    this->action.weapon = Green_Granade;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_h:
                    cout << "se apreto h -> Holy_Granade" << endl;
                    turn_worm.take_weapon(Holy_Granade);
                    this->action.type = Take_weapon;
                    this->action.weapon = Holy_Granade;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_m:
                    cout << "se apreto m -> Mortar" << endl;
                    turn_worm.take_weapon(Mortar);
                    this->action.type = Take_weapon;
                    this->action.weapon = Mortar;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_r:
                    cout << "se apreto r-> Red_Granade" << endl;
                    this->action.type = Take_weapon;
                    this->action.weapon = Red_Granade;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_t:
                    cout << "se apreto t -> Teletrans" << endl;
                    turn_worm.take_weapon(Teletrans);
                    this->wait_for_destination_clicl = true;
                    this->action.type = Take_weapon;
                    this->action.weapon = Teletrans;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_u:
                    cout << "se apreto u ->  Banana" << endl;
                    turn_worm.take_weapon(Banana);
                    this->action.type = Take_weapon;
                    this->action.weapon = Banana;
                    this->stage.make_action(this->action);
                    break;
                case SDLK_v:
                    cout << "se apreto v -> Baseboll_Ba" << endl;
                    turn_worm.take_weapon(Baseboll_Bat);
                    this->action.type = Take_weapon;
                    this->action.weapon = Baseboll_Bat;
                    this->stage.make_action(this->action);
                    break;
                }
                break;
    }
    return true;
    }

};


/////////////////////////////////////////////////////////////////////////////////////////////
class Graphic_Designer{
    TTF_Font *font;
    SDL_Color text_color;



public:

Graphic_Designer(int i){
    // Initialize SDL_ttf library
    if (TTF_Init() != 0) {
      cout << "TTF_Init() Failed: " << TTF_GetError() << endl;
      SDL_Quit();
      exit(1);
    }


    this->font = TTF_OpenFont("resources/Amiko-Bold.ttf", 10);
    if (this->font == NULL){
        cout << "TTF_OpenFont() Fail: " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    this->text_color =  {0, 0, 0};


}



void show_life(SDL_Surface * screen, int life, int x, int y, Color color){

    char str_life[10];
    if(life < 100){
        sprintf(str_life, " %d ", life);
    }else{
        sprintf(str_life, "%d", life);
    }


    SDL_Surface * text = TTF_RenderText_Solid(font,str_life,text_color);
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

////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////




class Weapons_Animation_Controller{
    Weapon_Name * current_weapon; 
    std::map<Weapon_Name,Animation> animations;

public:

Weapons_Animation_Controller(int i){
        this->current_weapon = NULL;

        Animation bazooka = Animation_Factory::get_bazooka();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Bazooka, bazooka));

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
        this->animations.insert(std::pair<Weapon_Name,Animation>(Air_Attack,air_attack));

        Animation dynamite = Animation_Factory::get_dynamite();
        this->animations.insert(std::pair<Weapon_Name,Animation>(Dynamite,dynamite));
}

void show_weapon( StageDTO s,SDL_Surface * screen){
    for (auto weapon_info: s.weapons) {

        debug_box2d_figure(screen, weapon_info);

        int up_left_vertex_x = get_pixels(weapon_info.x);
        int up_left_vertex_y = get_pixels(weapon_info.y);

        std::map<Weapon_Name,Animation>::iterator weapon_iter = animations.find(*this->current_weapon);
        weapon_iter->second.continue_internal_movement();
        weapon_iter->second.draw(screen,up_left_vertex_x, up_left_vertex_y);
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
        Worm_Animation_Controller worm(position_worm_x, position_worm_y);
        worms.insert(std::pair<int,Worm_Animation_Controller>(id,worm));
        worm.show(screen);

    }

    return worms;
}




void show_worms(StageDTO s, SDL_Surface *screen, std::map<int,Worm_Animation_Controller> & worms, Graphic_Designer & graphic_designer){

    for (auto w: s.worms) {

        ElementDTO worm_info = w.second;

        debug_box2d_figure(screen, worm_info);

        int up_left_vertex_x = get_pixels(worm_info.x);
        int up_left_vertex_y = get_pixels(worm_info.y);

        std::map<int,Worm_Animation_Controller>::iterator worms_iter = worms.find(w.first);
        worms_iter->second.move(up_left_vertex_x, up_left_vertex_y);
        worms_iter->second.show(screen);

        Color player_color = Color::create(Orange);
        int initial_life = 100;
        graphic_designer.show_life(screen, initial_life,up_left_vertex_x+20,up_left_vertex_y-5,player_color);

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

    Graphic_Designer graphic_designer(1);


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
    Event_Controller event_controller(event, stage);

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
            show_beams(s, screen);

            water.show(screen);
            //dibujo los gusanos
            show_worms(s, screen, worms, graphic_designer);
            weapons_controller.show_weapon(s, screen);

        }

    }

    return 0;
}
