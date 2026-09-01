#pragma once

#include "geometry/vec2.hpp"

#include <vector>

namespace croom {

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

}
