#pragma once

#include "game/collision/collision_bsp.hpp"

#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <string_view>
#include <vector>

namespace croom {

class MapLoader {
public:
  static CollisionBsp load(const std::filesystem::path &path, Vec2 player_position);

private:
  explicit MapLoader(const std::filesystem::path &path);

  static std::string_view trim(std::string_view value);
  [[noreturn]] void reject_row(std::string_view reason) const;
  float parse_coordinate(std::string_view token) const;
  float read_coordinate(std::string_view &row, bool has_more_coordinates) const;
  std::array<float, 4> parse_coordinates(std::string_view row) const;
  LineSegment parse_wall(std::string_view row) const;
  void read_rows();
  void check_player_position(const CollisionBsp &collisions, Vec2 position) const;

  std::filesystem::path path_;
  std::ifstream file_;
  std::vector<LineSegment> walls_;
  std::size_t line_ = 0;
};

}
