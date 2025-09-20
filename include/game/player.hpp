#pragma once
#include "game/entity.hpp"
#include "utils/vec2.hpp"
#include "core/user_input.hpp"

/** 
player class, it have propreties of player and few methods
*/
class Player: public Entity {
   private:
   /**
   players currect absolute position
   */
   Vec2 position;
   /**
   speed in which player can move
   */
   int speed;

   public:

   explicit Player(int speed);

   /** 
   according to @param input moves the player selected way
   */
   void move(UserInput input);

   /**
   gets player currect position
   @returns Vec2 of players position
   */
   Vec2 get_position();
};