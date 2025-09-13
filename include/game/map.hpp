#pragma once

#include "game/sprite.hpp"
#include "utils/vec2.hpp"
#include <memory>
#include <vector>
class Map {
   std::vector<std::unique_ptr<Sprite>> sprite_vec;
   Vec2 map_size;
   public:
      Map(Vec2 map_size);
      int add_entity(Sprite *sprite);
      void remove_entity(int entity_id);
      void clear_entities();
      Vec2 get_map_size();
      const std::vector<std::unique_ptr<Sprite>>& get_entity_vector();
};