#pragma once

#include "geometry/line_segment.hpp"

#include <cstdint>

namespace croom {

enum class WallSide : std::uint8_t { Front, Back, OnDivider };

class CollisionGeometry {
public:
  static constexpr float epsilon = 0.000001f;
  static constexpr float epsilon_squared = epsilon * epsilon;
  static constexpr float contact_slop = 0.001f;

  explicit CollisionGeometry(LineSegment wall);

  void validate() const;
  float length() const;
  bool is_degenerate() const;
  double signed_area(Vec2 point) const;
  double signed_distance(Vec2 point) const;
  WallSide classify(const LineSegment &wall) const;
  Vec2 closest_point(Vec2 point) const;
  bool has_clearance(Vec2 position, float radius) const;
  bool depenetrate(Vec2 &position, float radius, Vec2 &preferred_normal) const;

private:
  static double precise_dot(Vec2 left, Vec2 right);
  float projection_fraction(Vec2 point, Vec2 direction) const;
  Vec2 oriented_normal(Vec2 preferred_normal) const;
  Vec2 separation_normal(Vec2 position, Vec2 preferred_normal) const;
  float penetration_depth(Vec2 position, float radius) const;

  LineSegment wall_;
};

}
