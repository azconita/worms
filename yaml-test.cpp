//g++ -std=c++11 prueba-emmiter-yaml.cpp -lyaml-cpp -g

#include <iostream>
#include "DTOs.h"

using std::cout;
using std::endl;

int main(){

	StageDTO stage_sended;
	stage_sended.worm_turn = 1;

	ElementDTO beam1;
	beam1.pos_x = 32;
	beam1.pos_y = 21;
	beam1.h = 10;
	beam1.w = 30;
	beam1.angle = 98.9;

	stage_sended.beams.push_back(beam1);

	

	ElementDTO weapon1;
	weapon1.pos_x = 3;
	weapon1.pos_y = 2;
	weapon1.h = 10;
	weapon1.w = 30;
	weapon1.weapon =Banana;
	weapon1.timer = 3;

	ElementDTO explosion;
	explosion.pos_x = 3;
	explosion.pos_y = 2;
	explosion.h = 10;
	explosion.w = 30;
	explosion.weapon = Explosion;
	explosion.explosion = 2;
	
	
	stage_sended.weapons.push_back(weapon1);
	stage_sended.weapons.push_back(explosion);


	ElementDTO worm1;
	worm1.pos_x = 3;
	worm1.pos_y = 2;
	worm1.h = 10;
	worm1.w = 30;
	worm1.angle = 98.9;
	worm1.life = 100;
	worm1.player_id =0;

	ElementDTO worm2;
	worm2.pos_x = 3;
	worm2.pos_y = 2;
	worm2.h = 10;
	worm2.w = 30;
	worm2.angle = 0;
	worm2.life = 90;
	worm2.player_id =1;

	
	stage_sended.worms[1] = worm1;	
	stage_sended.worms[2] = worm2;

	YAML::Emitter out;
	out << YAML::BeginMap;
  	out << YAML::Key << "stage";
  	out << YAML::Value << stage_sended;
  	out << YAML::EndMap;

    cout << "se envia:\n'" << out.c_str() <<"'" << endl;


    YAML::Node yaml_received = YAML::Load(out.c_str());
    StageDTO stage_received = yaml_received ["stage"].as<StageDTO>();
    cout << "se recibe:" << endl;
   	cout << "turno:" << stage_received.worm_turn << endl;
   	cout << "beams:\n";
   	for(auto b: stage_received.beams){
  	  cout <<" { pos x: " << b.pos_x << ", pos y: " << b.pos_y << ", h: "
  	   << b.h <<", w: " << b.w <<  ", angle:" << b.angle << "}" << endl;
  
  	}
  	cout << "weapons:\n";	
  	for(auto we: stage_received.weapons){
  		cout << " { pos x: " << we.pos_x << ", pos y: " << we.pos_y << ", h: " 
  		<< we.h <<", w: " << we.w << ", weapon name: " <<  we.weapon<< ", explosion:" << we.explosion << "}" << endl;
  	}

  	cout << "worms:\n";

  	for(auto & pair: stage_received.worms){
    	cout << " worm id:  " << pair.first << endl;

    	ElementDTO w = pair.second;
    
   		cout << "  { x: "<< w.pos_x << " , y: "<< w.pos_y << ", h: " 
      	<< w.h <<", w: " << w.w <<  ", angle: " << w.angle <<", player id: "<< w.player_id << ", life: " << w.life <<" }"<< endl;

  	}	


}