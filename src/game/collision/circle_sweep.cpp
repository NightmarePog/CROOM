#include "game/collision/circle_sweep.hpp"

#include "game/collision/collision_geometry.hpp"

#include <algorithm>
#include <array>
#include <cmath>

using namespace std;

namespace croom {
namespace {
constexpr float time_epsilon = 0.00001f;
constexpr Vec2 unit_x{1.0f, 0.0f};
}

CircleSweep::CircleSweep(Vec2 start, Vec2 displacement, float radius)
    : start_{start}, displacement_{displacement}, radius_{radius} {}

optional<SweepHit> CircleSweep::hit() const { return earliest_; }

double CircleSweep::precise_dot(Vec2 left, Vec2 right) {
  return static_cast<double>(left.x) * right.x + static_cast<double>(left.y) * right.y;
}

bool CircleSweep::is_valid_hit(float time, Vec2 normal) const {
  const bool valid_time = time >= -time_epsilon && time <= 1.0f + time_epsilon;
  return valid_time && displacement_.dot(normal) < -CollisionGeometry::epsilon;
}

void CircleSweep::consider_hit(float time, Vec2 normal) {
  if (!is_valid_hit(time, normal))
    return;

  const SweepHit candidate{clamp(time, 0.0f, 1.0f), normal};
  if (!earliest_) {
    earliest_ = candidate;
    return;
  }

  const bool earlier = candidate.time < earliest_->time;
  const bool simultaneous = candidate.time == earliest_->time;
  const bool stronger =
      displacement_.dot(candidate.normal) < displacement_.dot(earliest_->normal);
  if (earlier || (simultaneous && stronger))
    earliest_ = candidate;
}

void CircleSweep::sweep_endpoint(Vec2 endpoint) {
  const Vec2 offset = start_ - endpoint;
  const double movement_squared = precise_dot(displacement_, displacement_);
  const double toward = precise_dot(offset, displacement_);
  const double radius_squared = static_cast<double>(radius_) * radius_;
  const double discriminant =
      toward * toward -
      movement_squared * (precise_dot(offset, offset) - radius_squared);
  if (movement_squared <= CollisionGeometry::epsilon_squared ||
      discriminant < -CollisionGeometry::epsilon)
    return;

  const float time =
      static_cast<float>((-toward - sqrt(max(0.0, discriminant))) / movement_squared);
  const Vec2 contact_offset = start_ + displacement_ * time - endpoint;
  const Vec2 fallback =
      (-displacement_).normalized_or(unit_x, CollisionGeometry::epsilon);
  consider_hit(time,
               contact_offset.normalized_or(fallback, CollisionGeometry::epsilon));
}

void CircleSweep::sweep_sides(const LineSegment &wall, const SegmentFrame &frame) {
  const float normal_movement = displacement_.dot(frame.normal);
  if (abs(normal_movement) <= CollisionGeometry::epsilon)
    return;

  for (float side : array{-1.0f, 1.0f}) {
    const float distance = (start_ - wall.start).dot(frame.normal);
    const float time = (side * radius_ - distance) / normal_movement;
    const float along = (start_ + displacement_ * time - wall.start).dot(frame.tangent);
    if (along >= -CollisionGeometry::epsilon &&
        along <= frame.length + CollisionGeometry::epsilon)
      consider_hit(time, frame.normal * side);
  }
}

void CircleSweep::consider(const LineSegment &wall) {
  const float wall_length = CollisionGeometry{wall}.length();
  if (wall_length <= CollisionGeometry::epsilon) {
    sweep_endpoint(wall.start);
    return;
  }

  const Vec2 tangent = wall.direction() / wall_length;
  sweep_sides(wall, {tangent, {-tangent.y, tangent.x}, wall_length});
  sweep_endpoint(wall.start);
  sweep_endpoint(wall.end);
}

}
