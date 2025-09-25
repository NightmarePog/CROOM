#include "game/map.hpp"
#include "game/player.hpp"
#include "game/sprite.hpp"
#include "utils/globals.hpp"
#include "utils/vec2.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Map::Map(Vec2 map_size) { this->map_size = map_size; }

void Map::remove_entity(int id) {
  if (id >= 0 && id < sprite_vec.size()) {
    sprite_vec.erase(sprite_vec.begin() + id);
  }
}

int Map::add_entity(Sprite *sprite) {
  sprite_vec.push_back(std::unique_ptr<Sprite>(sprite));
  return sprite_vec.size() - 1;
}

void Map::add_player(Player *player) {
  this->plr = player;
  this->plr->set_spawn(Vec2(64,64));
  this->plr->respawn();
}

Player* Map::get_player() {
  return this->plr;
}

void Map::remove_player() {
  delete this->plr;
}

void Map::clear_entities() { sprite_vec.clear(); }

std::vector<Sprite *> Map::get_entity_vector() {
  std::vector<Sprite *> result;
  result.reserve(sprite_vec.size());

  for (auto &e : sprite_vec) {
    result.push_back(e.get());
  }

  return result;
}

void Map::import_from_csv(const char *path) {
  int line[4];
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open CSV file");
  }

  std::string line_str;
  while (std::getline(file, line_str)) {
    std::istringstream ss(line_str);
    std::string token;
    float values[4];
    int i = 0;

    while (std::getline(ss, token, ',') && i < 4) {
      try {
        values[i] = std::stoi(token);
      } catch (...) {
        throw std::runtime_error("Invalid number in CSV");
      }
      i++;
    }

    if (i == 4) {
      add_entity(new Sprite(globals::renderer, "",
                            {{values[0], values[1]}, {values[2], values[3]}}));
    } else {
      throw std::runtime_error("CSV line does not have 4 values");
    }
  }
}
