#pragma once

#include "geometry/vec2.hpp"

namespace croom {

struct LineSegment {
  Vec2 start;
  Vec2 end;

  constexpr Vec2 direction() const { return end - start; }
};

}
