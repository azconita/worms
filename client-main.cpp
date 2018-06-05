


#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "Error.h"
#include "WaterAnimation.h"
#include "WormAnimation.h"
#include "EventController.h"
#include "stage.h"
#include "Beam.h"
#include "Worm.h"



using std::cout;
using std::endl;
using std::string;
using std::map;
using std::pair;

logger oLog("prueba.log");



#include "Error.h"
#include "Client.h"

#define SERVER_NAME 1 
#define PORT 2


int main(int argc, char * argv[]){
    if (argc < 3){
        throw Error("Parametros incorrectos");
    }
        
    Client cliente(argv[SERVER_NAME],argv[PORT]);
    cliente.run();
    return 0;
}
