#include "Renderer.h"


Renderer::Renderer(SDL_Surface *screen, BlockingQueue<StageDTO> & queue,GraphicDesigner & gd, int id):
	player_id(id),
	screen(screen),
	graphic_designer(gd),
	stages_queue(queue){
	this->on = true; 
}

void Renderer::start(){
	this->principal_thread = std::thread(&Renderer::draw, this);
}

void Renderer::draw(){
	StageDTO s;

	//para controlar el tiempo
    Uint32 t0 = SDL_GetTicks();
    Uint32 t1;

	bool finish = false;
	while(this->on){

        //actualiza el dibujo de la superficie en la pantalla
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();

        //update
        if(finish == false){
            s = stages_queue.pop(); 
            this->worm_turn = s.worm_turn;
            
        } 

        if((t1 -t0) > 17) {
         	// Nueva referencia de tiempo
         	t0 = SDL_GetTicks();
            //FIX ME
            if(s.winner != -1){
                	
                	finish = true;
                    
                    if(s.winner == this->player_id){
                            printf("GANE\n");
                            SDL_Flip(screen);
                            this->graphic_designer.won();         
                    }else{
                            printf("PERDIII\n");
                            SDL_Flip(screen);
                            this->graphic_designer.lost();
                    }
            } else {
                //borro todo lo que estaba
                //toda la pantalla en negro
                SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
                //dibujo las vigas y el agua
                this->graphic_designer.show_elements(s,screen);

            }

        }
    }
		   
}

int Renderer::get_worm_turn(){
	return this->worm_turn;
}


Renderer::~Renderer(){
	this->on= false;
	this->principal_thread.join();
}	
