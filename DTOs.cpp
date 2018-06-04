#include "DTOs.h"

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
  out << YAML::Key << "worm_turn";
  out << YAML::Value << s.worm_turn;
  out << YAML::Key << "beams";
  out << YAML::Flow << s.beams;
  out << YAML::Key << "weapons";
  out << YAML::Flow << s.weapons;
  out << YAML::Key << "worms";
  out << s.worms;
  out << YAML::EndMap;
  return out;
}