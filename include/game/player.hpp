#pragma once
#include "game/entity.hpp"
#include "utils/line_segment.hpp"
#include "utils/vec2.hpp"
#include "core/user_input.hpp"
#include <vector>

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
   /**
   currect player rotation
   */
   float rotation;
   /** 
   Field of view
   */
   int FOV;

   public:

   explicit Player(int FOV);

   /** 
   according to @param input moves the player selected way
   */
   void move(UserInput input);
   /**
   rotates the player according to @param input */
   void rotate(UserInput input);
   /**
   gets currect player rotation
   */
   float get_rotation();
   /**
   @returns player's FOV
   */
   int get_FOV();

   /**
   gets player currect position
   @returns Vec2 of players position
   */
   Vec2 get_position();

   /**
   @returns segments in field of view
   */
   std::vector<LineSegment> get_FOV_segments();
};