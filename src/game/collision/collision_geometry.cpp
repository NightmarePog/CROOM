#include "game/collision/collision_geometry.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

namespace croom {
namespace {
constexpr float side_epsilon = 0.0001f;
}

CollisionGeometry::CollisionGeometry(LineSegment wall) : wall_{wall} {}

void CollisionGeometry::validate() const {
  const Vec2 direction = wall_.direction();
  if (!wall_.start.is_finite() || !wall_.end.is_finite() || !direction.is_finite() ||
      !isfinite(direction.length()))
    throw invalid_argument("collision walls must contain finite coordinates");
}

float CollisionGeometry::length() const { return wall_.direction().length(); }

bool CollisionGeometry::is_degenerate() const { return length() <= epsilon; }

double CollisionGeometry::signed_area(Vec2 point) const {
  const Vec2 direction = wall_.direction();
  const Vec2 offset = point - wall_.start;
  return static_cast<double>(direction.x) * offset.y -
         static_cast<double>(direction.y) * offset.x;
}

double CollisionGeometry::signed_distance(Vec2 point) const {
  return signed_area(point) / length();
}

WallSide CollisionGeometry::classify(const LineSegment &wall) const {
  if (is_degenerate())
    return WallSide::OnDivider;
  const double start_distance = signed_distance(wall.start);
  const double end_distance = signed_distance(wall.end);
  if (start_distance > side_epsilon && end_distance > side_epsilon)
    return WallSide::Front;
  if (start_distance < -side_epsilon && end_distance < -side_epsilon)
    return WallSide::Back;
  return WallSide::OnDivider;
}

double CollisionGeometry::precise_dot(Vec2 left, Vec2 right) {
  return static_cast<double>(left.x) * right.x + static_cast<double>(left.y) * right.y;
}

float CollisionGeometry::projection_fraction(Vec2 point, Vec2 direction) const {
  const double divisor = precise_dot(direction, direction);
  const double fraction = precise_dot(point - wall_.start, direction) / divisor;
  return static_cast<float>(clamp(fraction, 0.0, 1.0));
}

Vec2 CollisionGeometry::closest_point(Vec2 point) const {
  const Vec2 direction = wall_.direction();
  if (direction.length() <= epsilon)
    return wall_.start;
  return wall_.start + direction * projection_fraction(point, direction);
}

Vec2 CollisionGeometry::oriented_normal(Vec2 preferred_normal) const {
  const Vec2 direction = wall_.direction();
  const Vec2 normal =
      Vec2{-direction.y, direction.x}.normalized_or({1.0f, 0.0f}, epsilon);
  return normal.dot(preferred_normal) < 0.0f ? -normal : normal;
}

Vec2 CollisionGeometry::separation_normal(Vec2 position, Vec2 preferred_normal) const {
  const Vec2 offset = position - closest_point(position);
  if (offset.length_squared() > epsilon_squared)
    return offset.normalized_or({1.0f, 0.0f}, epsilon);
  return oriented_normal(preferred_normal);
}

float CollisionGeometry::penetration_depth(Vec2 position, float radius) const {
  return radius - (position - closest_point(position)).length();
}

bool CollisionGeometry::has_clearance(Vec2 position, float radius) const {
  return penetration_depth(position, radius) <= 0.0f;
}

bool CollisionGeometry::depenetrate(Vec2 &position, float radius,
                                    Vec2 &preferred_normal) const {
  const float penetration = penetration_depth(position, radius);
  if (penetration <= 0.0f)
    return false;
  const Vec2 normal = separation_normal(position, preferred_normal);
  position += normal * (penetration + contact_slop);
  preferred_normal = normal;
  return true;
}

}
