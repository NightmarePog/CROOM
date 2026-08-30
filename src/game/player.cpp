#include "game/player.hpp"

#include "game/collision/collision_bsp.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

using namespace std;

namespace croom {
void Player::validate_update(const PlayerCommand &command, float delta_seconds) {
  const bool valid_command =
      isfinite(command.forward) && isfinite(command.strafe) && isfinite(command.turn);
  if (!valid_command || !isfinite(delta_seconds) || delta_seconds < 0.0f)
    throw invalid_argument("player update requires finite input and nonnegative time");
}

float Player::next_heading(float turn, float delta_seconds) const {
  const double turn_amount = static_cast<double>(clamp(turn, -1.0f, 1.0f)) *
                             turn_speed * static_cast<double>(delta_seconds);
  const float heading = heading_degrees_ + static_cast<float>(fmod(turn_amount, 360.0));
  const float wrapped = fmod(heading, 360.0f);
  return wrapped < 0.0f ? wrapped + 360.0f : wrapped;
}

Vec2 Player::movement_for(const PlayerCommand &command, float heading) const {
  const Vec2 axes{clamp(command.strafe, -1.0f, 1.0f),
                  clamp(command.forward, -1.0f, 1.0f)};
  const float radians = heading * (numbers::pi_v<float> / 180.0f);
  Vec2 movement{sin(radians) * axes.y + cos(radians) * axes.x,
                -cos(radians) * axes.y + sin(radians) * axes.x};
  const float length = movement.length();
  return length > 1.0f ? movement / length : movement;
}

void Player::update(const PlayerCommand &command, float delta_seconds,
                    const CollisionBsp &collisions) {
  validate_update(command, delta_seconds);
  const float heading = next_heading(command.turn, delta_seconds);
  const Vec2 movement =
      movement_for(command, heading) * (movement_speed * delta_seconds);
  const Vec2 position =
      collisions.resolve_movement(position_, movement, collision_radius);
  position_ = position;
  heading_degrees_ = heading;
}

}
