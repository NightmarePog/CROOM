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
   players current absolute position
   */
   Vec2 position;
   /**
   players absolute spawn position
   */
   Vec2 spawn_position;
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
   void move(std::vector<UserInput> input);
   /**
   rotates the player according to @param input */
   void rotate(std::vector<UserInput> input);
   /**
   gets currect player rotation
   */
   float get_rotation();
   /**
   @returns player's FOV
   */
   int get_FOV();
   /**
   respawns player (moves to spawn_position)
   */
   void respawn();
   /**
   sets spawn position of player
   */
   void set_spawn(Vec2 new_position);

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