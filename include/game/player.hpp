#pragma once
#include "game/entity.hpp"
#include "utils/vec2.hpp"

class Player: public Entity {
   private:
   Vec2 position;
   int speed;

   public:

   explicit Player(int speed);

   void move(Vec2);

   Vec2 get_position();
};