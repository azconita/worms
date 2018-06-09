#ifndef STAGELOADER_H_
#define STAGELOADER_H_


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
	std::vector<Worm_y> worms;
};

namespace YAML {
template<>
struct convert<Stage_y> {
  static Node encode(const Stage_y& s) {
  	Node node;
	node["beams"] = s.beams;
	node["worms"] = s.worms;
	
    return node;
  }
  static bool decode(const Node& node, Stage_y& s) {
  	s.beams = node["beams"].as<std::vector<Beam_y>>();
  	s.worms = node["worms"].as<std::vector<Worm_y>>();
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

class StageLoader{
	string file_name;
public:
	StageLoader(string file_name);
	Stage_y load_stage();
};

#endif
