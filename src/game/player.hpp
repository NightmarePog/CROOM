#pragma once

#include "geometry/vec2.hpp"

namespace croom {

class CollisionBsp;
class Map;

struct PlayerCommand {
  float forward = 0.0f;
  float strafe = 0.0f;
  float turn = 0.0f;
};

class Player {
public:
  static constexpr float collision_radius = 8.0f;
  static constexpr float movement_speed = 32.0f;
  static constexpr float turn_speed = 45.0f;
  static constexpr Vec2 default_spawn{72.0f, 64.0f};

  constexpr Vec2 position() const { return position_; }
  constexpr float heading_degrees() const { return heading_degrees_; }

private:
  friend class Map;

  void update(const PlayerCommand &command, float delta_seconds,
              const CollisionBsp &collisions);

  Vec2 movement_for(const PlayerCommand &command, float heading) const;
  static Vec2 command_axes(const PlayerCommand &command);
  static Vec2 rotate_axes(Vec2 axes, float heading);
  static Vec2 limit_length(Vec2 movement);
  float next_heading(float turn, float delta_seconds) const;
  static void validate_update(const PlayerCommand &command, float delta_seconds);

  Vec2 position_ = default_spawn;
  float heading_degrees_ = 0.0f;
};

}
