#include "game/player.hpp"
#include "utils/vec2.hpp"

Player::Player(int speed) {
  this->speed = speed;
  this->position = {64, 64};
    // TODO default position = map center
}

Vec2 Player::get_position() { return this->position; }

void Player::move(UserInput input) {
  Vec2 move_direction;
  switch (input) {

    case UserInput::W:
      move_direction = Vec2(0,-1);
      break;
    case UserInput::A:
      move_direction = Vec2(-1,0);
      break;
    case UserInput::S:
      move_direction = Vec2(0,1);
      break;
    case UserInput::D:
      move_direction = Vec2(1,0);
      break;
    case UserInput::QUIT:
    case UserInput::UNDEFINED:
    break;
  }
  position = position+move_direction;
}
