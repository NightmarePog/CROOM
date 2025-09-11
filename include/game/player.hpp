#pragma once
#include "utils/vec2.hpp"

class Player {
   private:
   Vec2 position;
   int speed;

   public:

   explicit Player(int speed);

   void move(Vec2);

   Vec2 get_position();
};