#pragma once

#include "geometry/line_segment.hpp"

#include <optional>
#include <span>

namespace croom {

struct RayHit {
  Vec2 position;
  float distance;
};

class Raycaster {
public:
  static std::optional<RayHit> cast(Vec2 origin, Vec2 direction,
                                    std::span<const LineSegment> walls);
};

}
