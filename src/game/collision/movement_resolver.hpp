#pragma once

#include "game/collision/circle_sweep.hpp"
#include "game/collision/collision_tree.hpp"
#include "game/collision/contact_manifold.hpp"

#include <cstddef>
#include <optional>
#include <vector>

namespace croom {

class MovementResolver {
public:
  MovementResolver(const CollisionTree &collisions, float radius);

  Vec2 resolve(Vec2 start, Vec2 displacement) const;

private:
  struct State {
    Vec2 position;
    Vec2 remaining;
    Vec2 verified;
    std::vector<std::size_t> candidates;
    ContactManifold contacts;
  };

  static constexpr int maximum_contact_iterations = 6;
  static constexpr int maximum_depenetration_iterations = 20;

  void validate(Vec2 start, Vec2 displacement) const;
  void find_candidates(Vec2 position, float radius,
                       std::vector<std::size_t> &candidates) const;
  bool has_clearance(Vec2 position, std::vector<std::size_t> &candidates) const;
  bool depenetrate(Vec2 &position, Vec2 preferred_normal,
                   std::vector<std::size_t> &candidates) const;
  std::optional<SweepHit> find_earliest_hit(State &state) const;
  bool resolve_hit(State &state, const SweepHit &hit) const;
  void resolve_unblocked(State &state) const;
  State initialize_state(Vec2 start, Vec2 displacement) const;
  bool needs_contact_resolution(const State &state) const;
  void resolve_contacts(State &state) const;
  Vec2 direct_result(const State &state) const;

  const CollisionTree &collisions_;
  float radius_;
};

}
