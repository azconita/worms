#include "Actioner.h"


Actioner::Actioner(Socket & socket, BlockingQueue<ActionDTO> & queue):
	socket(socket),
	actions_queue(queue){
	this->on = true;
}

void Actioner::start(){
	this->principal_thread = std::thread(&Actioner::make_action, this);
}

void Actioner::make_action(){
	extern logger oLog;
	 
	while(this->on){
		ActionDTO action = this->actions_queue.pop();
		if(action.type == Quit){
			oLog() << "player quit. ";
			break;
		}
    	YAML::Emitter out;
    	out << YAML::BeginMap;
    	out << YAML::Key << "action";
    	out << YAML::Value << action;
    	out << YAML::EndMap;
    	
    	oLog() << "sending ActionDTO:\n  "<< out.c_str();
    	(this->socket).send_dto(out.c_str());
    }
    
}


Actioner::~Actioner(){
	this->on= false;
	this->principal_thread.join();
}	
