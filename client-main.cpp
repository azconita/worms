#include "Logger.h"
#include "Error.h"
#include "Client.h"
#define SERVER_NAME 1 
#define PORT 2
#define LITTLE_SCREEN 3


logger oLog("client.log");

int main(int argc, char * argv[]){
    if (argc < 3){
        throw Error("Parametros incorrectos");
    }
    if(argc == 3){  
    	Client cliente(argv[SERVER_NAME],argv[PORT], true);
    	cliente.run();
	}else{
		Client cliente(argv[SERVER_NAME],argv[PORT], false);
    	cliente.run();
	}	
    
    return 0;
}
