#include "game/collision/collision_bsp.hpp"

#include "game/collision/movement_resolver.hpp"

#include <utility>

using namespace std;

namespace croom {

CollisionBsp::CollisionBsp() = default;

CollisionBsp::CollisionBsp(span<const LineSegment> walls)
    : CollisionBsp{vector<LineSegment>{walls.begin(), walls.end()}} {}

CollisionBsp::CollisionBsp(vector<LineSegment> walls) : tree_{std::move(walls)} {}

CollisionBsp::CollisionBsp(CollisionBsp &&) = default;

CollisionBsp &CollisionBsp::operator=(CollisionBsp &&) = default;

CollisionBsp::~CollisionBsp() = default;

void CollisionBsp::rebuild(span<const LineSegment> walls) {
  rebuild(vector<LineSegment>{walls.begin(), walls.end()});
}

void CollisionBsp::rebuild(vector<LineSegment> walls) {
  tree_.rebuild(std::move(walls));
}

span<const LineSegment> CollisionBsp::walls() const { return tree_.walls(); }

Vec2 CollisionBsp::resolve_movement(Vec2 start, Vec2 displacement, float radius) const {
  return MovementResolver{tree_, radius}.resolve(start, displacement);
}

}
