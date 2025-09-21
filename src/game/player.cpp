#include "game/player.hpp"
#include "core/user_input.hpp"
#include "utils/globals.hpp"
#include "utils/vec2.hpp"
#include <cmath>
#include <cstdio>
#include <iostream>

Player::Player(int fov) {
  this->speed = 16;
  this->position = {64, 64};
    // TODO default position = map center
  this->rotation = 0;
  this->FOV = fov;
}

Vec2 Player::get_position() { return this->position; }

void Player::move(UserInput input) {
    Vec2 move_direction{0, 0};

    switch (input) {
        case UserInput::W: move_direction = Vec2(0, -1); break;
        case UserInput::A: move_direction = Vec2(-1, 0); break;
        case UserInput::S: move_direction = Vec2(0, 1); break;
        case UserInput::D: move_direction = Vec2(1, 0); break;
        case UserInput::QUIT:
        case UserInput::UNDEFINED:
        case UserInput::ARROW_LEFT:
        case UserInput::ARROW_RIGHT:
            return; 
    }

    // TODO right rotation align
    float rotation_rad = this->rotation * (M_PI / 180.0f);
    float rotated_x = move_direction.x * cos(rotation_rad) - move_direction.y * sin(rotation_rad) * constants::FRAME_INTERVAL;
    float rotated_y = move_direction.x * sin(rotation_rad) + move_direction.y * cos(rotation_rad) * constants::FRAME_INTERVAL;

    Vec2 rotated_direction{rotated_x, rotated_y};

    this->position = position + rotated_direction;
}


int Player::get_FOV() {
  return this->FOV;
}

float Player::get_rotation() {
  return this->rotation;
}

void Player::rotate(UserInput input) {
  switch (input) {

    case UserInput::ARROW_LEFT:
      this->rotation += 2.0*constants::FRAME_INTERVAL;
      break;
    case UserInput::ARROW_RIGHT:
      // 
      this->rotation += -2.0*constants::FRAME_INTERVAL;
      printf("%f\n", this->rotation);
      break;
    case UserInput::W:
    case UserInput::A:
    case UserInput::S:
    case UserInput::D:
    case UserInput::QUIT:
    case UserInput::UNDEFINED:
      break;
  }
}