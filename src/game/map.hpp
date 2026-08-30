#pragma once

#include "game/collision/collision_bsp.hpp"
#include "game/player.hpp"

#include <filesystem>
#include <span>

namespace croom {

class Map {
public:
  void load_from_csv(const std::filesystem::path &path);
  void update(const PlayerCommand &command, float delta_seconds) {
    player_.update(command, delta_seconds, collisions_);
  }

  constexpr const Player &player() const { return player_; }
  std::span<const LineSegment> walls() const { return collisions_.walls(); }

private:
  Player player_;
  CollisionBsp collisions_;
};

}
