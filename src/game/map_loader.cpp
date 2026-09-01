#include "game/map_loader.hpp"

#include "game/map_row_parser.hpp"
#include "game/player.hpp"

#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

namespace croom {
namespace {
constexpr float position_tolerance = 0.0001f;
}

MapLoader::MapLoader(const filesystem::path &path) : path_(path), file_(path) {
  if (!file_.is_open()) {
    throw runtime_error("Could not open map: " + path_.string());
  }
}

CollisionBsp MapLoader::load(const filesystem::path &path, Vec2 player_position) {
  MapLoader loader{path};
  loader.read_rows();
  CollisionBsp collisions{std::move(loader.walls_)};
  loader.check_player_position(collisions, player_position);
  return collisions;
}

void MapLoader::read_rows() {
  string row;
  while (getline(file_, row)) {
    ++line_;
    if (!MapRowParser::is_blank(row))
      walls_.push_back(MapRowParser{line_}.parse(row));
  }
  if (!file_.eof())
    throw runtime_error("Could not read map: " + path_.string());
}

void MapLoader::check_player_position(const CollisionBsp &collisions,
                                      Vec2 position) const {
  const Vec2 safe_position =
      collisions.resolve_movement(position, {}, Player::collision_radius);
  if ((safe_position - position).length() > position_tolerance) {
    throw runtime_error("Map places the player inside a wall: " + path_.string());
  }
}

}
