/*
 * DTOs.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: gabi
 */

#include "Dtos.h"

#include "yaml-cpp/yaml.h"
#include <map>
#include <vector>
#include <string>




YAML::Emitter& operator << (YAML::Emitter& out, const ElementDTO& e) {
  out << YAML::BeginMap;
  out << YAML::Key << "pos_x";
  out << YAML::Value << e.pos_x;
  out << YAML::Key <<  "pos_y";
  out << YAML::Value << e.pos_y;
  out << YAML::Key <<  "h";
  out << YAML::Value << e.h;
  out << YAML::Key <<  "w";
  out << YAML::Value << e.w;
  out << YAML::Key << "worm_state";
  out << YAML::Value << e.worm_state;
  out << YAML::Key << "direction";
  out << YAML::Value << e.direction;
  out << YAML::Key <<  "angle";
  out << YAML::Value << e.angle;
  out << YAML::Key <<  "life";
  out << YAML::Value << e.life;
  out << YAML::Key <<  "player_id";
  out << YAML::Value << e.player_id;
  out << YAML::Key <<  "weapon";
  out << YAML::Value << e.weapon;
  out << YAML::Key <<  "timer";
  out << YAML::Value << e.timer;
  out << YAML::Key <<  "explosion";
  out << YAML::Value << e.explosion;
  out << YAML::EndMap;
  return out;
}


YAML::Emitter& operator << (YAML::Emitter& out, const StageDTO& s) {
  out << YAML::BeginMap;
  out << YAML::Key << "player_id";
  out << YAML::Value << s.player_id;
  out << YAML::Key << "worm_turn";
  out << YAML::Value << s.worm_turn;
  out << YAML::Key << "beams";
  out << YAML::Value << s.beams;
  out << YAML::Key << "weapons";
  out << YAML::Value << s.weapons;
  out << YAML::Key << "worms";
  out << YAML::Value << s.worms;
  out << YAML::Key << "winner";
  out << YAML::Value << s.winner;
  out << YAML::EndMap;
  return out;
}


YAML::Emitter& operator << (YAML::Emitter& out, const ActionDTO& a) {
  out << YAML::BeginMap;
  out << YAML::Key << "worm_id";
  out << YAML::Value << a.worm_id;
  out << YAML::Key << "type";
  out << YAML::Value << a.type;
  out << YAML::Key << "move";
  out << YAML::Value << a.move;
  out << YAML::Key << "weapon";
  out << YAML::Value << a.weapon;
  out << YAML::Key << "weapon_degrees";
  out << YAML::Value << a.weapon_degrees;
  out << YAML::Key << "power";
  out << YAML::Value << a.power;
  out << YAML::Key << "time_to_explode";
  out << YAML::Value << a.time_to_explode;
  out << YAML::Key << "direction";
  out << YAML::Value << a.direction;
  out << YAML::Key << "pos_x";
  out << YAML::Value << a.pos_x;
  out << YAML::Key <<  "pos_y";
  out << YAML::Value << a.pos_y;
  out << YAML::EndMap;
  return out;
}


