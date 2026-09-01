#pragma once

#include "geometry/line_segment.hpp"

#include <array>
#include <cstddef>
#include <string_view>

namespace croom {

class MapRowParser {
public:
  explicit MapRowParser(std::size_t line);

  LineSegment parse(std::string_view row) const;
  static bool is_blank(std::string_view row);

private:
  static std::string_view trim(std::string_view value);
  [[noreturn]] void reject(std::string_view reason) const;
  float parse_coordinate(std::string_view token) const;
  float read_coordinate(std::string_view &row, bool has_more) const;
  std::array<float, 4> parse_coordinates(std::string_view row) const;

  std::size_t line_;
};

}
