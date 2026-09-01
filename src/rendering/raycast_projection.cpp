#include "rendering/raycast_projection.hpp"

#include "rendering/raycaster.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

using namespace std;

namespace croom {
namespace {
constexpr float field_of_view_degrees = 70.0f;
constexpr float wall_height = 64.0f;
constexpr float minimum_projection_distance = 0.01f;

float to_radians(float degrees) {
  return degrees * (numbers::pi_v<float> / 180.0f);
}

float normalized_screen_x(int x, int width) {
  return (static_cast<float>(x) + 0.5f) /
             (static_cast<float>(width) * 0.5f) -
         1.0f;
}

void validate(Viewport viewport) {
  if (viewport.width <= 0 || viewport.height <= 0)
    throw invalid_argument("raycast projection requires a positive viewport");
}
}

RaycastProjection::RaycastProjection(RaycastCamera camera, Viewport viewport)
    : camera_{camera}, viewport_{viewport},
      heading_radians_{to_radians(camera.heading_degrees)},
      half_fov_tangent_{tan(to_radians(field_of_view_degrees * 0.5f))},
      focal_length_{static_cast<float>(viewport.width) * 0.5f / half_fov_tangent_} {
  validate(viewport);
}

float RaycastProjection::angle_offset(int x) const {
  return atan(normalized_screen_x(x, viewport_.width) * half_fov_tangent_);
}

Vec2 RaycastProjection::ray_direction(float angle_offset) const {
  const float angle = heading_radians_ + angle_offset;
  return {sin(angle), -cos(angle)};
}

float RaycastProjection::corrected_distance(float distance,
                                            float angle_offset) const {
  return max(distance * cos(angle_offset), minimum_projection_distance);
}

float RaycastProjection::projected_height(float distance) const {
  return wall_height * focal_length_ / distance;
}

int RaycastProjection::wall_top(float height) const {
  return static_cast<int>(
      max(0.0f, (static_cast<float>(viewport_.height) - height) * 0.5f));
}

int RaycastProjection::wall_bottom(float height) const {
  return static_cast<int>(
      min(static_cast<float>(viewport_.height - 1),
          (static_cast<float>(viewport_.height) + height) * 0.5f));
}

optional<ProjectedWallColumn>
RaycastProjection::project_column(int x, span<const LineSegment> walls) const {
  const float offset = angle_offset(x);
  const optional<RayHit> hit =
      Raycaster::cast(camera_.position, ray_direction(offset), walls);
  if (!hit)
    return nullopt;

  const float distance = corrected_distance(hit->distance, offset);
  const float height = projected_height(distance);
  return ProjectedWallColumn{x, wall_top(height), wall_bottom(height), distance};
}

vector<ProjectedWallColumn>
RaycastProjection::project(span<const LineSegment> walls) const {
  vector<ProjectedWallColumn> columns;
  columns.reserve(static_cast<size_t>(viewport_.width));

  for (int x = 0; x < viewport_.width; ++x) {
    const optional<ProjectedWallColumn> column = project_column(x, walls);
    if (column)
      columns.push_back(*column);
  }
  return columns;
}

}
