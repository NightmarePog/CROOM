#pragma once

#include "geometry/line_segment.hpp"

#include <optional>

namespace croom {

struct SweepHit {
  float time = 1.0f;
  Vec2 normal;
};

class CircleSweep {
public:
  CircleSweep(Vec2 start, Vec2 displacement, float radius);

  void consider(const LineSegment &wall);
  std::optional<SweepHit> hit() const;

private:
  struct SegmentFrame {
    Vec2 tangent;
    Vec2 normal;
    float length;
  };

  static double precise_dot(Vec2 left, Vec2 right);
  bool is_valid_hit(float time, Vec2 normal) const;
  void consider_hit(float time, Vec2 normal);
  void sweep_endpoint(Vec2 endpoint);
  void sweep_sides(const LineSegment &wall, const SegmentFrame &frame);

  Vec2 start_;
  Vec2 displacement_;
  float radius_;
  std::optional<SweepHit> earliest_;
};

}
