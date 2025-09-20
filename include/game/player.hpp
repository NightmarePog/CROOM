#pragma once
#include "game/entity.hpp"
#include "utils/vec2.hpp"
#include "core/user_input.hpp"

class Player: public Entity {
   private:
   Vec2 position;
   int speed;

   public:

   explicit Player(int speed);

   void move(UserInput input);

   Vec2 get_position();
};