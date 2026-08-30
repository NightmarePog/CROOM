#include "game/map_loader.hpp"

#include "game/player.hpp"

#include <charconv>
#include <cmath>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>

using namespace std;

namespace croom {
namespace {
constexpr string_view ascii_whitespace = " \t\r\n\f\v";
constexpr float minimum_wall_length = 0.001f;
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

string_view MapLoader::trim(string_view value) {
  const size_t first = value.find_first_not_of(ascii_whitespace);
  const size_t last = value.find_last_not_of(ascii_whitespace);
  return first == string_view::npos ? string_view{}
                                    : value.substr(first, last - first + 1);
}

[[noreturn]] void MapLoader::reject_row(string_view reason) const {
  throw runtime_error("Invalid map row " + to_string(line_) + ": " + string{reason});
}

float MapLoader::parse_coordinate(string_view token) const {
  if (token.empty()) {
    reject_row("missing coordinate");
  }
  float coordinate = 0.0f;
  const auto result = from_chars(token.begin(), token.end(), coordinate);
  if (result.ec != errc{} || result.ptr != token.end() || !isfinite(coordinate)) {
    reject_row("coordinate is not a finite number");
  }
  return coordinate;
}

float MapLoader::read_coordinate(string_view &row, bool has_more_coordinates) const {
  const size_t separator = row.find(',');
  if (has_more_coordinates && separator == string_view::npos) {
    reject_row("missing coordinate");
  }
  if (!has_more_coordinates && separator != string_view::npos) {
    reject_row("extra coordinate");
  }
  const string_view token = row.substr(0, separator);
  row = separator == string_view::npos ? string_view{} : row.substr(separator + 1);
  return parse_coordinate(trim(token));
}

array<float, 4> MapLoader::parse_coordinates(string_view row) const {
  return {read_coordinate(row, true), read_coordinate(row, true),
          read_coordinate(row, true), read_coordinate(row, false)};
}

LineSegment MapLoader::parse_wall(string_view row) const {
  const auto [start_x, start_y, end_x, end_y] = parse_coordinates(row);
  const LineSegment wall{{start_x, start_y}, {end_x, end_y}};
  if (wall.direction().length() <= minimum_wall_length) {
    reject_row("wall has no length");
  }
  return wall;
}

void MapLoader::read_rows() {
  string row;
  while (getline(file_, row)) {
    ++line_;
    if (!trim(row).empty()) {
      walls_.push_back(parse_wall(row));
    }
  }
  if (!file_.eof()) {
    throw runtime_error("Could not read map: " + path_.string());
  }
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
