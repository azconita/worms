/*
 * yaml-test.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: gabi
 */

#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"
#include "DTOs.h"

std::string get_yaml(StageDTO &s);
StageDTO get_stageDTO();

int main(int argc, char* argv[]) {
  StageDTO s = get_stageDTO();
  std::string str = get_yaml(s);
  std::cout << str;
}

std::string get_yaml(StageDTO &s) {
  YAML::Node node;
  node["worm_turn"] = s.worm_turn;
  YAML::Node worms;
  for (auto &w : s.worms) {
    worms[w.first] = w.second;
  }
  node["worms"] = worms;
  for (auto &b : s.beams) {
    node["beams"].push_back(b);
  }
  for (auto &b : s.beams) {
    node["weapons"].push_back(b);
  }
  std::string str;
  node >> str;
  return str;
}

StageDTO get_stageDTO() {
  StageDTO s;
  s.worm_turn = 0;
  ElementDTO elem;
  elem.x = 1;
  elem.y = 2;
  elem.h = 3;
  elem.w = 4;
  elem.life = 5;
  elem.player_id = 6;
  //hacer encode/decode de Weapon_Name
  elem.weapon = 7;
  elem.timer = 8;
  elem.explosion = true;

  s.worms[0] = elem;
  s.beams.push_back(elem);
  s.weapons.push_back(elem);

  return s;
}
