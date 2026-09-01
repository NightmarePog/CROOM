#pragma once

#include "geometry/line_segment.hpp"

#include <optional>
#include <span>
#include <vector>

namespace croom {

struct Viewport {
  int width;
  int height;
};

struct RaycastCamera {
  Vec2 position;
  float heading_degrees;
};

struct ProjectedWallColumn {
  int x;
  int top;
  int bottom;
  float distance;
};

class RaycastProjection {
public:
  RaycastProjection(RaycastCamera camera, Viewport viewport);

  std::vector<ProjectedWallColumn>
  project(std::span<const LineSegment> walls) const;

private:
  std::optional<ProjectedWallColumn>
  project_column(int x, std::span<const LineSegment> walls) const;

  float angle_offset(int x) const;
  Vec2 ray_direction(float angle_offset) const;
  float corrected_distance(float distance, float angle_offset) const;
  float projected_height(float distance) const;
  int wall_top(float height) const;
  int wall_bottom(float height) const;

  RaycastCamera camera_;
  Viewport viewport_;
  float heading_radians_;
  float half_fov_tangent_;
  float focal_length_;
};

}
