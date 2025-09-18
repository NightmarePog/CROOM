#include "core/2d_render.hpp"
#include "game/map.hpp"
#include "game/player.hpp"
#include "game/sprite.hpp"
#include <SDL_render.h>
#include <iostream>

void render(SDL_Renderer *renderer, Map *map) {
  if (!renderer)
    return;
  const auto &sprite_vec = map->get_entity_vector();
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (size_t i = 0; i < sprite_vec.size(); ++i) {
    LineSegment cords = map->get_entity_vector()[i]->get_cords();
    SDL_RenderDrawLine(renderer, cords.pos_a.x, cords.pos_a.y, cords.pos_b.x,
                       cords.pos_b.y);
  }
  Player *plr = map->get_player();
  std::cout << "PLR POSITION:" << plr->get_position().x << "," << plr->get_position().y << std::endl;
  SDL_Rect rect {100, 100, static_cast<int>(plr->get_position().x), static_cast<int>(plr->get_position().y)};
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // zelená
  SDL_RenderFillRect(renderer, &rect);
};
