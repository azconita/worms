#include "Logger.h"
#include "Error.h"
#include "Client.h"
#define SERVER_NAME 1 
#define PORT 2


logger oLog("client.log");

int main(int argc, char * argv[]){
    if (argc < 3){
        throw Error("Parametros incorrectos");
    }
        
    Client cliente(argv[SERVER_NAME],argv[PORT]);
    cliente.run();
    return 0;
}
