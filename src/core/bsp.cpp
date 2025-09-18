#include "core/bsp.hpp"
#include "game/map.hpp"
#include "game/sprite.hpp"
#include "utils/line_segment.hpp"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <vector>


void printBSPTree(const BSPNode* node, const std::string& prefix = "", bool isFront = true) {
    if (!node) return;

    std::cout << prefix;
    std::cout << (isFront ? "├──" : "└──");

    std::cout << "[";
    std::cout << "Segments: " << node->segments.size() << "]\n";

    std::string newPrefix = prefix + (isFront ? "│   " : "    ");
    printBSPTree(node->front.get(), newPrefix, true);
    printBSPTree(node->back.get(), newPrefix, false);
}

BSP::BSP() {
  this->root = new BSPNode{
      nullptr, nullptr, new Partition(LineSegment{{0, 0}, {0, 0}}), {}};
}

void BSP::load_from_map(Map *map) {
  std::vector<Sprite *> sprites = map->get_entity_vector();
  for (Sprite *sprite : sprites) {
    this->root->segments.push_back(sprite->get_cords());
  }
  this->is_loaded = true;
  std::vector<Sprite *> spritess = map->get_entity_vector();
}

void BSP::build_bsp() {
    if (!this->is_loaded) {
        throw std::runtime_error("no map loaded for BSP creation");
    }
    if (this->root->segments.empty()) {
        throw std::runtime_error("no segments in BSP root");
    }

      LineSegment parti = get_longest_segment(this->root->segments);

  
    this->root->partition = new Partition(get_longest_segment(this->root->segments));

    this->queue.push(this->root);
    while (!this->queue.empty()) {
      process_node(this->queue.front());
      this->queue.pop();
    }
    std::cout << "BSP tree created!" << std::endl;
    // check if output is correct
    // basically tests just idk how -w-
    printBSPTree(root);
}

LineSegment BSP::get_longest_segment(std::vector<LineSegment> segments) {

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
      longest = *const_cast<LineSegment*>(&seg);
    }
  }

  return longest;
}


void BSP::process_node(BSPNode *node) {
  node->partition = new Partition(get_longest_segment(node->segments)); 
  node->front = std::make_unique<BSPNode>();
  node->back = std::make_unique<BSPNode>();
  for (LineSegment &segment : node->segments) {
    Side result = node->partition->decide_side(segment);
    switch (result) {
      case Side::BACK:
        node->back->segments.push_back(segment);
        break;
      case Side::FRONT:
        node->front->segments.push_back(segment);
        break;
      //case Side::BETWEEN:
        // TODO - tbh i have no idea how to handle these....
        //break;
      }
  }
  if (node->back->segments.size() < 1) {
    this->queue.push(node->back.get());
  }
  if (node->front->segments.size() < 1) {
    this->queue.push(node->front.get());
  }
}