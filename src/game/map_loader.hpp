#pragma once

#include "game/collision/collision_bsp.hpp"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>

namespace croom {

class MapLoader {
public:
  static CollisionBsp load(const std::filesystem::path &path, Vec2 player_position);

private:
  explicit MapLoader(const std::filesystem::path &path);

  void read_rows();
  void check_player_position(const CollisionBsp &collisions, Vec2 position) const;

  std::filesystem::path path_;
  std::ifstream file_;
  std::vector<LineSegment> walls_;
  std::size_t line_ = 0;
};

}
