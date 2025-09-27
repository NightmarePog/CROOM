#include "core/2d_render.hpp"
#include "game/map.hpp"
#include "game/player.hpp"
#include "game/sprite.hpp"
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstddef>
#include <stdexcept>

SDL_Texture* create_solid_texture(SDL_Renderer* renderer, int size, SDL_Color color) {
    SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size, size);
    SDL_SetRenderTarget(renderer, tex);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
    return tex;
}

void render(SDL_Renderer *renderer, Map *map) {
  if (!renderer)
    throw std::runtime_error("No render found");
  const auto &sprite_vec = map->get_entity_vector();
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (size_t i = 0; i < sprite_vec.size(); ++i) {
    LineSegment cords = map->get_entity_vector()[i]->get_cords();
    SDL_RenderDrawLine(renderer, cords.pos_a.x, cords.pos_a.y, cords.pos_b.x,
                       cords.pos_b.y);
  }
  Player *plr = map->get_player();
  SDL_Rect dstRect {
      static_cast<int>(plr->get_position().x - 8),
      static_cast<int>(plr->get_position().y - 8),
      16,
      16
  };
  // useful for debug showing rotation
  SDL_Rect arrowRect {
      static_cast<int>(plr->get_position().x - 16),
      static_cast<int>(plr->get_position().y - 2),
      32,
      4
  };
  SDL_Texture *tex = create_solid_texture(renderer, 16, SDL_Color(255,0,0,255));
  SDL_RenderCopyEx(renderer, tex, nullptr, &arrowRect, -plr->get_rotation(), nullptr, SDL_FLIP_NONE);
  SDL_RenderCopyEx(renderer, tex, nullptr, &dstRect, -plr->get_rotation(), nullptr, SDL_FLIP_NONE);
  SDL_DestroyTexture(tex);
}