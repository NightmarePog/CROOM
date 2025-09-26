#pragma once
#include "game/entity.hpp"
#include "utils/line_segment.hpp"
#include "utils/vec2.hpp"
#include "core/user_input.hpp"
#include <vector>
#include <queue>

/** 
player class, it have propreties of player and few methods
*/
class Player: public Entity {
   private:
   /**
   players current absolute position
   */
   Vec2 position;
   /**
   speed in which player can move
   */
   int speed;
   /**
   current player rotation
   */
   float rotation;
   /**
   speed in which player can rotate
   */
   int rotation_speed;
   /** 
   Field of view
   */
   int FOV;

   public:

   explicit Player(int FOV);

   /** 
   according to @param input moves the player selected way
   */
   void move(std::queue<UserInput> input);
   /**
   rotates the player according to @param input */
   void rotate(std::queue<UserInput> input);
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