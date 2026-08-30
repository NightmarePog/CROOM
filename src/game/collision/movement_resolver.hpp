#pragma once

#include "game/collision/collision_tree.hpp"

#include <cstddef>
#include <optional>
#include <vector>

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

class ContactManifold {
public:
  ContactManifold();

  void add(Vec2 normal);
  Vec2 clip(Vec2 movement) const;

private:
  bool contains(Vec2 normal) const;
  bool blocks(Vec2 movement) const;

  std::vector<Vec2> normals_;
};

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
  Vec2 direct_result(const State &state) const;

  const CollisionTree &collisions_;
  float radius_;
};

}
