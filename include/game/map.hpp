#pragma once

#include "game/player.hpp"
#include "game/sprite.hpp"
#include "utils/vec2.hpp"
#include <memory>
#include <vector>
class Map {
   std::vector<std::unique_ptr<Sprite>> sprite_vec;
   Vec2 map_size;
   Player *plr;
   public:
      Map(Vec2 map_size);
      /**
      adds entity into map - used for walls
      @param *sprite - entity you want to add
      @returns entity index in array
       */
      int add_entity(Sprite *sprite);
      /**
      remove entity with id from vector
      @param entity_id index of the entity to remove
      */
      void remove_entity(int entity_id);
      /**
      clears every entity in game
      */
      void clear_entities();
      /**
      import all entities from CSV file
      */
      void import_from_csv(const char* path);
      /**
      @returns map size 
      */
      Vec2 get_map_size();

      /**
      @returns whole vector of entities
      */
      const std::vector<std::unique_ptr<Sprite>>& get_entity_vector();
};