#pragma once

#include "game/collision/collision_tree.hpp"

#include <span>
#include <vector>

namespace croom {

class CollisionBsp {
public:
  CollisionBsp();
  explicit CollisionBsp(std::span<const LineSegment> walls);
  explicit CollisionBsp(std::vector<LineSegment> walls);

  CollisionBsp(const CollisionBsp &) = delete;
  CollisionBsp &operator=(const CollisionBsp &) = delete;
  CollisionBsp(CollisionBsp &&);
  CollisionBsp &operator=(CollisionBsp &&);
  ~CollisionBsp();

  void rebuild(std::span<const LineSegment> walls);
  void rebuild(std::vector<LineSegment> walls);

  std::span<const LineSegment> walls() const;
  Vec2 resolve_movement(Vec2 start, Vec2 displacement, float radius) const;

private:
  CollisionTree tree_;
};

}
