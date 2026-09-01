#include "game/collision/collision_tree.hpp"

#include <numeric>
#include <utility>

using namespace std;

namespace croom {

CollisionTree::CollisionTree() = default;

CollisionTree::CollisionTree(vector<LineSegment> walls) { rebuild(std::move(walls)); }

CollisionTree::CollisionTree(CollisionTree &&) = default;

CollisionTree &CollisionTree::operator=(CollisionTree &&) = default;

CollisionTree::~CollisionTree() = default;

void CollisionTree::validate_walls(span<const LineSegment> walls) {
  for (const LineSegment &wall : walls)
    CollisionGeometry{wall}.validate();
}

void CollisionTree::rebuild(vector<LineSegment> walls) {
  validate_walls(walls);
  vector<size_t> wall_indices(walls.size());
  iota(wall_indices.begin(), wall_indices.end(), size_t{0});
  auto root = Builder{walls}.build(std::move(wall_indices));
  walls_ = std::move(walls);
  root_ = std::move(root);
}

span<const LineSegment> CollisionTree::walls() const { return walls_; }

bool CollisionTree::empty() const { return !root_; }

void CollisionTree::query(Vec2 center, float radius, vector<size_t> &results) const {
  results.clear();
  query_node(root_.get(), center, radius, results);
}

void CollisionTree::query_node(const Node *node, Vec2 center, float radius,
                               vector<size_t> &results) const {
  if (!node)
    return;
  results.insert(results.end(), node->on_divider.begin(), node->on_divider.end());
  const CollisionGeometry divider{walls_[node->divider_index]};
  if (divider.is_degenerate()) {
    query_node(node->front.get(), center, radius, results);
    query_node(node->back.get(), center, radius, results);
    return;
  }
  const double distance = divider.signed_distance(center);
  if (distance >= -radius)
    query_node(node->front.get(), center, radius, results);
  if (distance <= radius)
    query_node(node->back.get(), center, radius, results);
}

}
