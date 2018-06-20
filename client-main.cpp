#include "Logger.h"
#include "Error.h"
#include "Client.h"
#define SERVER_NAME 1 
#define PORT 2
#define STAGE_NAME 3
#define LITTLE_SCREEN 4

logger oLog("client.log");

int main(int argc, char * argv[]){
    if (argc < 4){
        throw Error("Parametros incorrectos");
    }
    std::string stage(argv[STAGE_NAME]);
    if(argc == 4){
    	Client client(argv[SERVER_NAME],argv[PORT], stage, true);
    	client.run();
	}else{
		Client client(argv[SERVER_NAME],argv[PORT], stage, true);
    	client.run();
	}	
    
    return 0;
}
