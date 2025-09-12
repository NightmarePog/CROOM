#pragma once
#include "game/map.hpp"
#include "game/player.hpp"

class Game {
   private:
   Map map;
   Player player;

   public:
   Game(Map map, Player plr);

   void tick();
   
};