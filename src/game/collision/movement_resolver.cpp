#include "game/collision/movement_resolver.hpp"

#include "game/collision/collision_geometry.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

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

ContactManifold::ContactManifold() { normals_.reserve(2); }

bool ContactManifold::contains(Vec2 normal) const {
  for (Vec2 existing : normals_)
    if (existing.dot(normal) > 0.999f)
      return true;
  return false;
}

void ContactManifold::add(Vec2 normal) {
  if (!contains(normal))
    normals_.push_back(normal);
}

bool ContactManifold::blocks(Vec2 movement) const {
  for (Vec2 normal : normals_)
    if (movement.dot(normal) < -CollisionGeometry::epsilon)
      return true;
  return false;
}

Vec2 ContactManifold::clip(Vec2 movement) const {
  for (size_t pass = 0; pass < normals_.size(); ++pass) {
    for (Vec2 normal : normals_) {
      const float depth = movement.dot(normal);
      if (depth < 0.0f)
        movement -= normal * depth;
    }
  }
  return blocks(movement) ? Vec2{} : movement;
}

MovementResolver::MovementResolver(const CollisionTree &collisions, float radius)
    : collisions_{collisions}, radius_{radius} {}

void MovementResolver::validate(Vec2 start, Vec2 displacement) const {
  if (!start.is_finite() || !displacement.is_finite() || !isfinite(radius_) ||
      radius_ <= 0.0f)
    throw invalid_argument(
        "collision movement requires finite coordinates and a positive radius");
  if (!isfinite(displacement.length()))
    throw invalid_argument("collision displacement is too large");
}

void MovementResolver::find_candidates(Vec2 position, float radius,
                                       vector<size_t> &candidates) const {
  collisions_.query(position, radius, candidates);
}

bool MovementResolver::has_clearance(Vec2 position, vector<size_t> &candidates) const {
  find_candidates(position, radius_ + CollisionGeometry::contact_slop, candidates);
  for (size_t wall_index : candidates)
    if (!CollisionGeometry{collisions_.walls()[wall_index]}.has_clearance(position,
                                                                          radius_))
      return false;
  return true;
}

bool MovementResolver::depenetrate(Vec2 &position, Vec2 preferred_normal,
                                   vector<size_t> &candidates) const {
  for (int iteration = 0; iteration < maximum_depenetration_iterations; ++iteration) {
    find_candidates(position, radius_ + CollisionGeometry::contact_slop, candidates);
    bool overlap = false;
    for (size_t wall_index : candidates)
      overlap |= CollisionGeometry{collisions_.walls()[wall_index]}.depenetrate(
          position, radius_, preferred_normal);
    if (!overlap)
      return true;
  }
  return has_clearance(position, candidates);
}

optional<SweepHit> MovementResolver::find_earliest_hit(State &state) const {
  const float remaining_length = state.remaining.length();
  find_candidates(state.position + state.remaining * 0.5f,
                  radius_ + remaining_length * 0.5f + CollisionGeometry::contact_slop,
                  state.candidates);
  CircleSweep sweep{state.position, state.remaining, radius_};
  for (size_t wall_index : state.candidates)
    sweep.consider(collisions_.walls()[wall_index]);
  return sweep.hit();
}

bool MovementResolver::resolve_hit(State &state, const SweepHit &hit) const {
  state.position += state.remaining * hit.time;
  state.position += hit.normal * CollisionGeometry::contact_slop;
  state.remaining *= 1.0f - hit.time;
  state.contacts.add(hit.normal);
  state.remaining = state.contacts.clip(state.remaining);
  if (!depenetrate(state.position, hit.normal, state.candidates) ||
      !has_clearance(state.position, state.candidates))
    return false;
  state.verified = state.position;
  return true;
}

void MovementResolver::resolve_unblocked(State &state) const {
  const Vec2 destination = state.position + state.remaining;
  if (has_clearance(destination, state.candidates)) {
    state.position = destination;
    state.verified = destination;
  }
  state.remaining = {};
}

Vec2 MovementResolver::direct_result(const State &state) const {
  const Vec2 result = state.position + (collisions_.empty() ? state.remaining : Vec2{});
  if (!result.is_finite())
    throw invalid_argument("collision movement result is not finite");
  return result;
}

Vec2 MovementResolver::resolve(Vec2 start, Vec2 displacement) const {
  validate(start, displacement);
  State state{start, displacement, start, {}, {}};
  if (!depenetrate(state.position, -state.remaining, state.candidates))
    throw runtime_error("collision depenetration did not converge");
  state.verified = state.position;
  if (collisions_.empty() ||
      state.remaining.length_squared() <= CollisionGeometry::epsilon_squared)
    return direct_result(state);
  for (int iteration = 0; iteration < maximum_contact_iterations; ++iteration) {
    if (state.remaining.length() <= CollisionGeometry::epsilon)
      break;
    const optional<SweepHit> earliest = find_earliest_hit(state);
    if (!earliest) {
      resolve_unblocked(state);
      break;
    }
    if (!resolve_hit(state, *earliest))
      break;
  }
  return state.verified;
}

}
