#include "game/collision/collision_tree.hpp"

#include <numeric>
#include <utility>

using namespace std;

namespace croom {

CollisionTree::Builder::Builder(span<const LineSegment> walls) : walls_{walls} {}

WallSide CollisionTree::Builder::side_of(size_t wall_index,
                                         size_t divider_index) const {
  return CollisionGeometry{walls_[divider_index]}.classify(walls_[wall_index]);
}

CollisionTree::SideCounts
CollisionTree::Builder::count_sides(span<const size_t> wall_indices,
                                    size_t divider_index) const {
  SideCounts counts;
  for (size_t wall_index : wall_indices) {
    switch (side_of(wall_index, divider_index)) {
    case WallSide::Front:
      ++counts.front;
      break;
    case WallSide::Back:
      ++counts.back;
      break;
    case WallSide::OnDivider:
      ++counts.on_divider;
      break;
    }
  }
  return counts;
}

size_t CollisionTree::Builder::score(span<const size_t> wall_indices,
                                     size_t divider_index) const {
  const SideCounts counts = count_sides(wall_indices, divider_index);
  const size_t imbalance = counts.front > counts.back ? counts.front - counts.back
                                                      : counts.back - counts.front;
  return imbalance + counts.on_divider * 2;
}

size_t CollisionTree::Builder::choose_divider(span<const size_t> wall_indices) const {
  DividerChoice choice{wall_indices.front(), score(wall_indices, wall_indices.front())};
  for (size_t candidate : wall_indices) {
    const size_t candidate_score = score(wall_indices, candidate);
    if (candidate_score < choice.score)
      choice = {candidate, candidate_score};
  }
  return choice.index;
}

CollisionTree::Partitions
CollisionTree::Builder::partition(span<const size_t> wall_indices,
                                  size_t divider_index) const {
  Partitions partitions;
  for (size_t wall_index : wall_indices) {
    switch (side_of(wall_index, divider_index)) {
    case WallSide::Front:
      partitions.front.push_back(wall_index);
      break;
    case WallSide::Back:
      partitions.back.push_back(wall_index);
      break;
    case WallSide::OnDivider:
      partitions.on_divider.push_back(wall_index);
      break;
    }
  }
  return partitions;
}

unique_ptr<CollisionTree::Node>
CollisionTree::Builder::build(vector<size_t> wall_indices) const {
  if (wall_indices.empty())
    return nullptr;
  const size_t divider_index = choose_divider(wall_indices);
  Partitions partitions = partition(wall_indices, divider_index);
  auto front = build(std::move(partitions.front));
  auto back = build(std::move(partitions.back));
  return make_unique<Node>(Node{divider_index, std::move(partitions.on_divider),
                                std::move(front), std::move(back)});
}

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
