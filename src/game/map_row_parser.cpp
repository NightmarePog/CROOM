#include "game/map_row_parser.hpp"

#include <charconv>
#include <cmath>
#include <stdexcept>
#include <string>
#include <system_error>

using namespace std;

namespace croom {
namespace {
constexpr string_view ascii_whitespace = " \t\r\n\f\v";
constexpr float minimum_wall_length = 0.001f;
}

MapRowParser::MapRowParser(size_t line) : line_{line} {}

string_view MapRowParser::trim(string_view value) {
  const size_t first = value.find_first_not_of(ascii_whitespace);
  if (first == string_view::npos)
    return {};
  return value.substr(first, value.find_last_not_of(ascii_whitespace) - first + 1);
}

bool MapRowParser::is_blank(string_view row) { return trim(row).empty(); }

[[noreturn]] void MapRowParser::reject(string_view reason) const {
  throw runtime_error("Invalid map row " + to_string(line_) + ": " + string{reason});
}

float MapRowParser::parse_coordinate(string_view token) const {
  if (token.empty())
    reject("missing coordinate");

  float coordinate = 0.0f;
  const auto result = from_chars(token.begin(), token.end(), coordinate);
  if (result.ec != errc{} || result.ptr != token.end() || !isfinite(coordinate))
    reject("coordinate is not a finite number");
  return coordinate;
}

float MapRowParser::read_coordinate(string_view &row, bool has_more) const {
  const size_t separator = row.find(',');
  if (has_more && separator == string_view::npos)
    reject("missing coordinate");
  if (!has_more && separator != string_view::npos)
    reject("extra coordinate");

  const string_view token = row.substr(0, separator);
  row = separator == string_view::npos ? string_view{} : row.substr(separator + 1);
  return parse_coordinate(trim(token));
}

array<float, 4> MapRowParser::parse_coordinates(string_view row) const {
  return {read_coordinate(row, true), read_coordinate(row, true),
          read_coordinate(row, true), read_coordinate(row, false)};
}

LineSegment MapRowParser::parse(string_view row) const {
  const auto [start_x, start_y, end_x, end_y] = parse_coordinates(row);
  const LineSegment wall{{start_x, start_y}, {end_x, end_y}};
  if (wall.direction().length() <= minimum_wall_length)
    reject("wall has no length");
  return wall;
}

}
