#include "game/collision/contact_manifold.hpp"

#include "game/collision/collision_geometry.hpp"

using namespace std;

namespace croom {

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

}
