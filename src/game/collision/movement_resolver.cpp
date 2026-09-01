#include "game/collision/movement_resolver.hpp"

#include "game/collision/collision_geometry.hpp"

#include <cmath>
#include <stdexcept>

using namespace std;

namespace croom {
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

MovementResolver::State MovementResolver::initialize_state(Vec2 start,
                                                           Vec2 displacement) const {
  State state{start, displacement, start, {}, {}};
  if (!depenetrate(state.position, -state.remaining, state.candidates))
    throw runtime_error("collision depenetration did not converge");
  state.verified = state.position;
  return state;
}

bool MovementResolver::needs_contact_resolution(const State &state) const {
  return !collisions_.empty() &&
         state.remaining.length_squared() > CollisionGeometry::epsilon_squared;
}

void MovementResolver::resolve_contacts(State &state) const {
  for (int iteration = 0; iteration < maximum_contact_iterations; ++iteration) {
    if (state.remaining.length() <= CollisionGeometry::epsilon)
      return;

    const optional<SweepHit> earliest = find_earliest_hit(state);
    if (!earliest) {
      resolve_unblocked(state);
      return;
    }
    if (!resolve_hit(state, *earliest))
      return;
  }
}

Vec2 MovementResolver::direct_result(const State &state) const {
  const Vec2 result = state.position + (collisions_.empty() ? state.remaining : Vec2{});
  if (!result.is_finite())
    throw invalid_argument("collision movement result is not finite");
  return result;
}

Vec2 MovementResolver::resolve(Vec2 start, Vec2 displacement) const {
  validate(start, displacement);
  State state = initialize_state(start, displacement);
  if (!needs_contact_resolution(state))
    return direct_result(state);

  resolve_contacts(state);
  return state.verified;
}

}
