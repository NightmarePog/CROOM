#include "game/player.hpp"
#include "core/user_input.hpp"
#include "utils/globals.hpp"
#include "utils/vec2.hpp"
#include <cmath>
#include <cstdlib>
#include <vector>

Player::Player(int fov) {
  this->speed = 16;
  this->position = {0,0};
  this->spawn_position = {0,0};
  this->rotation = 0;
  this->rotation_speed = 20;
  this->FOV = fov;
}

Vec2 Player::get_position() { return this->position; }

void Player::move(std::vector<UserInput> input) {
    Vec2 move_direction{0, 0};

    for (UserInput i : input) {
      switch (i) {
        case UserInput::W: move_direction = move_direction + Vec2(0, -1); break;
        case UserInput::A: move_direction = move_direction + Vec2(-1, 0); break;
        case UserInput::S: move_direction = move_direction + Vec2(0, 1); break;
        case UserInput::D: move_direction = move_direction + Vec2(1, 0); break;
        case UserInput::ARROW_LEFT:
        case UserInput::ARROW_RIGHT:
        case UserInput::QUIT:
        case UserInput::UNDEFINED:
            if (move_direction == 0) return;
            break;
      }
      // slow down if moving diagonal
      if (abs(move_direction.x)+abs(move_direction.y)>1) {
        move_direction = move_direction*0.5;
      }
    }

    float rotation_rad = this->rotation * (M_PI / 180.0f);
    float rotated_x = (move_direction.x * sin(rotation_rad) - move_direction.y * cos(rotation_rad)) * constants::FRAME_INTERVAL;
    float rotated_y = (move_direction.x * cos(rotation_rad) + move_direction.y * sin(rotation_rad)) * constants::FRAME_INTERVAL;

    Vec2 rotated_direction{rotated_x*Player::speed, rotated_y*Player::speed};

    this->position = position + rotated_direction;
}


int Player::get_FOV() {
  return this->FOV;
}

float Player::get_rotation() {
  return this->rotation;
}

void Player::rotate(std::vector<UserInput> input) {
  for (UserInput i : input) {
    switch (i) {
      case UserInput::ARROW_LEFT:
        this->rotation += this->rotation_speed*constants::FRAME_INTERVAL;
        if (this->rotation > 360) {
          this->rotation -= 360;
        }
        break;
      case UserInput::ARROW_RIGHT:
        this->rotation += -this->rotation_speed*constants::FRAME_INTERVAL;
        if (this->rotation < 0) {
          this->rotation += 360;
        }
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
}

void Player::respawn() {
  Player::position = Player::spawn_position;
}

void Player::set_spawn(Vec2 new_position) {
  Player::spawn_position = new_position;
}