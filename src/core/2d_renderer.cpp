#include "core/2d_render.hpp"
#include "game/map.hpp"
#include "game/sprite.hpp"
#include <SDL_render.h>

void render(SDL_Renderer *renderer, Map *map) {
  if (!renderer)
    return;
  const auto &sprite_vec = map->get_entity_vector(); // reference, ne kopie
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (size_t i = 0; i < sprite_vec.size(); ++i) {
    LineSegment cords = map->get_entity_vector()[i]->get_cords();
    SDL_RenderDrawLine(renderer, cords.pos_a.x, cords.pos_a.y, cords.pos_b.x,
                       cords.pos_b.y);
  }
};
