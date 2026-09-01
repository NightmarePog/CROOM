#include "game/collision/collision_tree.hpp"

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

}
