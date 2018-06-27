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
    std::chrono::nanoseconds diff(0);
    

	bool finish = false;
	while(this->on){

        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::nanoseconds(SLEEPNANOSECONDS) - diff);


        //actualiza el dibujo de la superficie en la pantalla
        SDL_Flip(screen);

        // Referencia de tiempo
        t1 = SDL_GetTicks();

        //update
        if(finish == false){
            s = stages_queue.pop(); 
            this->worm_turn = s.worm_turn;
            if(s.winner != -1){
                finish = true;
            }
        } 


       // if((t1 -t0) > 17) {
         	// Nueva referencia de tiempo
         	t0 = SDL_GetTicks();
            if(s.winner != -1){
                    
                    if(s.winner == this->player_id){
                            SDL_Flip(screen);
                            this->graphic_designer.won();         
                    }else{
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

       // }

        auto end = std::chrono::high_resolution_clock::now();

        diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start) - std::chrono::nanoseconds(SLEEPNANOSECONDS);
        diff = (diff.count() > 0) ? diff : std::chrono::nanoseconds(0);

    }
		   
}

int Renderer::get_worm_turn(){
	return this->worm_turn;
}


Renderer::~Renderer(){
	this->on= false;
	this->principal_thread.join();
}	
