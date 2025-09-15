#include "core/bsp.hpp"
#include "game/map.hpp"
#include "game/sprite.hpp"
#include "utils/line_segment.hpp"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>


BSP::BSP() {
  this->root = BSPNode{
      nullptr, nullptr, LineSegment{{0, 0}, {0, 0}}, {}};
  std::cout << "BSP loaded :3" << std::endl;
}

void BSP::load_from_map(Map *map) {
  std::vector<Sprite *> sprites = map->get_entity_vector();
  for (Sprite *sprite : sprites) {
    this->root.segments.push_back(sprite->get_cords());
  }
  this->is_loaded = true;
  std::vector<Sprite *> spritess = map->get_entity_vector();
}

void BSP::build_bsp() {
    if (!this->is_loaded) {
        throw std::runtime_error("no map loaded for BSP creation");
    }
    if (this->root.segments.empty()) {
        throw std::runtime_error("no segments in BSP root");
    }

    this->root.splitter = get_longest_segment(this->root.segments);

    // TODO node classification
}

LineSegment BSP::get_longest_segment(std::vector<LineSegment> segments) {
  printf("[GET LONGEST] %f\n", segments.front().pos_a.x);
  return LineSegment{{0, 0}, {0, 0}};
  if (segments.empty())
    throw std::runtime_error("No segments");

  LineSegment longest = segments.at(0);
  float max_length = 0.0f;

  // counting
  for (const LineSegment &seg : segments) {
    Vec2 delta = seg.pos_a - seg.pos_b;
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (length > max_length) {
      max_length = length;
      longest = *const_cast<LineSegment *>(&seg);
    }
  }

  return longest;
}
