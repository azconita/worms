/*
 * Picture.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: jaz
 */

//#define BOOST_LOG_DYN_LINK 1
#include "Picture.h"

SDL_Rect Picture::get_dimention(){
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


SDL_Surface * Picture::flip(SDL_Surface * original, Uint32 colorkey){
        SDL_Rect origen;
        SDL_Rect destino;


        //BOOST_LOG_TRIVIAL(trace) << "Se crea una imagen";

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
            throw Error("No podemos convertir la imagen al formato de pantalla");
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




Picture::Picture(const char * bmp_path, Colour color, int columns, int rows){
        this->rows = rows;
        this->columns = columns;
        this->row_num = 0;
        this->column_num = 0;
        this->default_direction = Left; //todas las fotos estan para la izquierda

        SDL_Surface *tmp =  SDL_LoadBMP(bmp_path);
        //SDL_Surface *tmp = IMG_Load(bmp_path);
        //tmp = rotozoomSurface(tmp,90.0, 10,0);     
        if (!tmp) {
            throw Error("Couldn't create surface from image: %s %s",bmp_path,SDL_GetError());
        }

        this->surface = SDL_DisplayFormat(tmp);
        SDL_FreeSurface(tmp);

        if(this->surface == NULL) {
            throw Error("Error: ", SDL_GetError());
        }

        // Calculamos el color transparente
         Uint32 colorkey = SDL_MapRGB(this->surface->format, color.r, color.g, color.b);
        // Lo establecemos como color transparente
        SDL_SetColorKey(this->surface, SDL_SRCCOLORKEY, colorkey);

        this->flipped = flip(surface, colorkey);

        // El ancho de una original es el total entre el número de columnas
        this->w = surface->w / columns;
        // El alto de una original es el total entre el número de filas
        this->h = surface->h / rows;
}
int Picture::get_height(){
    return this->h;
}

int Picture::get_width(){
    return this->w;
}

void Picture::draw(SDL_Surface *screen, int x, int y){
    SDL_Rect position;
    position.x = x - (this->w/2);
    position.y = y - (this->h/2);
    draw(screen, position, this->default_direction);
}

void Picture::debug(SDL_Surface *screen, std::vector<Vertex> v){
     SDL_Rect rectangle;
        rectangle.x = v[0].pos_x;
        rectangle.y = v[0].pos_y;
        rectangle.h = 5;
        rectangle.w = 5; 
        Uint32 colorkey = SDL_MapRGB(screen->format, 255,0,0);
        SDL_FillRect(screen, &rectangle, colorkey);

        rectangle.x = v[1].pos_x;
        rectangle.y = v[1].pos_y;
        rectangle.h = 5;
        rectangle.w = 5; 
        SDL_FillRect(screen, &rectangle, colorkey);

        rectangle.x = v[2].pos_x;
        rectangle.y = v[2].pos_y;
        rectangle.h = 5;
        rectangle.w = 5; 
        SDL_FillRect(screen, &rectangle, colorkey);

        rectangle.x = v[3].pos_x;
        rectangle.y = v[3].pos_y;
        rectangle.h = 5;
        rectangle.w = 5; 
        SDL_FillRect(screen, &rectangle, colorkey);

}


void Picture::draw(SDL_Surface *screen, std::vector<Vertex> v, Direction direction){
    this->debug(screen, v);

    float h = sqrt(pow(v[2].pos_x - v[1].pos_x,2) + pow(v[2].pos_y - v[1].pos_y,2));
    float w = sqrt(pow(v[1].pos_x - v[0].pos_x,2) + pow(v[1].pos_y - v[0].pos_y,2));

    SDL_Rect position;
    position.x = v[0].pos_x + 13; //correccion empirica de las vigas 
    position.y = v[0].pos_y; 
    position.h = h;
    position.w = w;
    draw(screen, position, direction);
}


void Picture::draw(SDL_Surface *screen, SDL_Rect position, Direction direction){
    SDL_Rect dimention = get_dimention();

    position.h =  dimention.h;
    position.w = dimention.w;

     if(this->default_direction != direction){
        SDL_BlitSurface(this->flipped, &dimention, screen, &position);
        return;
    }

    SDL_BlitSurface(this->surface, &dimention, screen, &position);

}


void Picture::next_sprite_figure(Direction direction){
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

void Picture::previous_sprite_figure(Direction direction){
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

bool Picture::is_in_first_figure(Direction direction){
    if(this->default_direction != direction){
        return (this->row_num <= 0 && this->column_num >= this->columns-1);
    }
    return (this->row_num <= 0 && this->column_num <= 0);

}

bool Picture::is_in_last_figure(Direction direction){
    if(this->default_direction != direction){
        return (this->row_num >= this->rows-1 && this->column_num <= 0);
    }
    return (this->row_num >= this->rows-1 && this->column_num >= this->columns-1);

}


Picture::~Picture() {   
    //SDL_FreeSurface(this->surface);
	
}

