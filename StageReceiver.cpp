
#include "StageReceiver.h"


StageReceiver::StageReceiver(Socket & socket, BlockingQueue<StageDTO> & queue):
	socket(socket),
	stages_queue(queue){
	this->on = true;
}

void StageReceiver::start(){
	this->principal_thread = std::thread(&StageReceiver::receive_stage, this);
}

void StageReceiver::receive_stage(){
	 
	while(this->on){
	    string stage_str = (this->socket).receive_dto();
	    YAML::Node yaml_received = YAML::Load(stage_str);
	    //printf("%s\n", stage_str.c_str());
	    StageDTO stage_received = yaml_received["stage"].as<StageDTO>();
	    (this->stages_queue).push(stage_received);
    }
    
}


StageReceiver::~StageReceiver(){
	this->on= false;
	this->principal_thread.join();
}	
