#include "rendering/raycaster.hpp"

#include <cmath>

using namespace std;

namespace croom {
namespace {
constexpr double parallel_epsilon = 0.000000001;
constexpr float direction_epsilon = 0.000001f;

double precise_cross(Vec2 left, Vec2 right) {
  return static_cast<double>(left.x) * right.y -
         static_cast<double>(left.y) * right.x;
}

optional<float> intersection_distance(Vec2 origin, Vec2 direction,
                                      const LineSegment &wall) {
  const Vec2 wall_direction = wall.direction();
  const double divisor = precise_cross(direction, wall_direction);
  if (abs(divisor) <= parallel_epsilon)
    return nullopt;

  const Vec2 wall_offset = wall.start - origin;
  const double ray_distance = precise_cross(wall_offset, wall_direction) / divisor;
  const double wall_fraction = precise_cross(wall_offset, direction) / divisor;
  if (ray_distance < 0.0 || wall_fraction < 0.0 || wall_fraction > 1.0)
    return nullopt;
  return static_cast<float>(ray_distance);
}

optional<Vec2> normalize_ray_direction(Vec2 direction) {
  if (!direction.is_finite())
    return nullopt;

  const float length = direction.length();
  if (length <= direction_epsilon)
    return nullopt;
  return direction / length;
}

optional<float> nearest_intersection(Vec2 origin, Vec2 direction,
                                     span<const LineSegment> walls) {
  optional<float> nearest;
  for (const LineSegment &wall : walls) {
    const optional<float> candidate = intersection_distance(origin, direction, wall);
    if (candidate && (!nearest || *candidate < *nearest))
      nearest = candidate;
  }
  return nearest;
}

RayHit make_hit(Vec2 origin, Vec2 direction, float distance) {
  return {origin + direction * distance, distance};
}
}

optional<RayHit> Raycaster::cast(Vec2 origin, Vec2 direction,
                                 span<const LineSegment> walls) {
  if (!origin.is_finite())
    return nullopt;

  const optional<Vec2> ray_direction = normalize_ray_direction(direction);
  if (!ray_direction)
    return nullopt;

  const optional<float> distance = nearest_intersection(origin, *ray_direction, walls);
  if (!distance)
    return nullopt;
  return make_hit(origin, *ray_direction, *distance);
}

}
