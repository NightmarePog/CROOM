#pragma once

#include "game/collision/collision_geometry.hpp"

#include <cstddef>
#include <memory>
#include <span>
#include <vector>

namespace croom {

class CollisionTree {
public:
  CollisionTree();
  explicit CollisionTree(std::vector<LineSegment> walls);
  CollisionTree(const CollisionTree &) = delete;
  CollisionTree &operator=(const CollisionTree &) = delete;
  CollisionTree(CollisionTree &&);
  CollisionTree &operator=(CollisionTree &&);
  ~CollisionTree();

  void rebuild(std::vector<LineSegment> walls);
  std::span<const LineSegment> walls() const;
  bool empty() const;
  void query(Vec2 center, float radius, std::vector<std::size_t> &results) const;

private:
  struct Node {
    std::size_t divider_index = 0;
    std::vector<std::size_t> on_divider;
    std::unique_ptr<Node> front;
    std::unique_ptr<Node> back;
  };

  struct SideCounts {
    std::size_t front = 0;
    std::size_t back = 0;
    std::size_t on_divider = 0;
  };

  struct Partitions {
    std::vector<std::size_t> front;
    std::vector<std::size_t> back;
    std::vector<std::size_t> on_divider;
  };

  class Builder {
  public:
    explicit Builder(std::span<const LineSegment> walls);
    std::unique_ptr<Node> build(std::vector<std::size_t> wall_indices) const;

  private:
    struct DividerChoice {
      std::size_t index;
      std::size_t score;
    };

    WallSide side_of(std::size_t wall_index, std::size_t divider_index) const;
    SideCounts count_sides(std::span<const std::size_t> wall_indices,
                           std::size_t divider_index) const;
    std::size_t score(std::span<const std::size_t> wall_indices,
                      std::size_t divider_index) const;
    std::size_t choose_divider(std::span<const std::size_t> wall_indices) const;
    Partitions partition(std::span<const std::size_t> wall_indices,
                         std::size_t divider_index) const;

    std::span<const LineSegment> walls_;
  };

  static void validate_walls(std::span<const LineSegment> walls);
  void query_node(const Node *node, Vec2 center, float radius,
                  std::vector<std::size_t> &results) const;

  std::vector<LineSegment> walls_;
  std::unique_ptr<Node> root_;
};

}
