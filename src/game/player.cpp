#include "game/player.hpp"

Player::Player(int speed) {
   this->speed = speed;
   this->position = {0,0};
}

Vec2 Player::get_position() {
   return this->position;
}

void Player::move(Vec2 by_vector) {
   Vec2 const move_by = {speed*by_vector.x, speed*by_vector.y};
   this->position = move_by+this->position;
   return;
}
