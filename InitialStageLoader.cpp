#include <iostream> 
#include <string> 
#include <fstream>
#include <yaml-cpp/yaml.h>

using std::cout;
using std::endl;
using std::string;
 


struct Worm_y{
	int id;
	double pos_x;
	double pos_y;
	int direction;
	double inclination;
	int life;
};

struct Beam_y{
	double pos_x;
	double pos_y;
	int size;
	double inclination;
};

struct Stage_y{
	std::vector<Beam_y> beams;
	std::map<int, std::vector<Worm_y>> players;
};

namespace YAML {
template<>
struct convert<Stage_y> {
  static Node encode(const Stage_y& s) {
  	Node node;
	node["beams"] = s.beams;
	node["players"] = s.players;
	
    return node;
  }
  static bool decode(const Node& node, Stage_y& s) {
  	s.beams = node["beams"].as<std::vector<Beam_y>>();
  	s.players = node["players"].as<std::map<int, std::vector<Worm_y>>>();
    return true;
  }
};

template<>
struct convert<Beam_y> {
  static Node encode(const Beam_y& b) {
    Node node;
    node["pos_x"] = b.pos_x;
    node["pos_y"] = b.pos_y;
    node["size"] = b.size;
    node["inclination"] = b.inclination;
 
    return node;
  }

  static bool decode(const Node& node, Beam_y& beam) {
    beam.pos_x = node["pos_x"].as<double>();
    beam.pos_y = node["pos_y"].as<double>();
    beam.size = node["size"].as<int>();
    beam.inclination = node["inclination"].as<double>();
    return true;
  }
};


template<>
struct convert<Worm_y> {
  static Node encode(const Worm_y& w) {
    Node node;
    node["id"] = w.id;
    node["pos_x"] = w.pos_x;
    node["pos_y"] = w.pos_y;
    node["direction"] = w.direction;
    node["life"] = w.life;
    node["inclination"] = w.inclination;
    return node;
  }

  static bool decode(const Node& node, Worm_y& worm) {
  	worm.id = node["id"].as<int>();
    worm.pos_x = node["pos_x"].as<double>();
    worm.pos_y = node["pos_y"].as<double>();
    worm.direction = node["direction"].as<int>();
    worm.life = node["life"].as<int>();
    worm.inclination = node["inclination"].as<double>();
    return true;
  }
};

}
	
int main(){
	YAML::Node initial_stage = YAML::LoadFile("file.yaml");
	Stage_y s = initial_stage["stage"].as<Stage_y>();
	for(auto b: s.beams){
		cout << "se levanto: beam_y { x: " << b.pos_x << ", y: " << b.pos_y << ", size: " << b.size << ", inclination:" << b.inclination << "}" << endl;
	}
	for(auto & pair: s.players){
		cout << "player  " << pair.first << endl;
		for(auto w : pair.second){
    
			cout << "worms: { id: "<< w.id << ", x: "<< w.pos_x << " , y: "
      << w.pos_y << ", direction: "<< w.direction << ", inclination: "<<w.inclination << ", life: " << w.life <<" }"<< endl;
		}
	}


	/*Beam_y end;
	end.pos_x = 10;
	end.pos_y = 20;
	end.size = 3;
	end.inclination = 90;
	s.beams.push_back(end);
	initial_stage["stage"] = s;

	

	std::ofstream file("file.yaml"); 
	
	file << initial_stage;
	

    file.close();*/


}


