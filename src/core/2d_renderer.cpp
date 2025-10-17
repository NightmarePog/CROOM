#include "core/2d_render.hpp"
#include "core/bsp.hpp"
#include "game/map.hpp"
#include "game/player.hpp"
#include "game/sprite.hpp"
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstddef>
#include <SDL_ttf.h>
#include <memory>
#include <queue>
#include <stdexcept>

SDL_Texture* create_solid_texture(SDL_Renderer* renderer, int size, SDL_Color color) {
    SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size, size);
    SDL_SetRenderTarget(renderer, tex);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
    return tex;
}

void render_lines(SDL_Renderer *renderer, BSPNode *bsp, TTF_Font* font) {
    if (!renderer) {
        throw std::runtime_error("no renderer found");
    }

    if (!bsp) return;

    std::queue<BSPNode*> node_queue;
    node_queue.push(bsp);

    while (!node_queue.empty()) {
        BSPNode *curr = node_queue.front();
        node_queue.pop();

        int index = 0;
        for (const auto &x : curr->segments) {
            // nakresli čáru
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderDrawLine(renderer, x.pos_a.x, x.pos_a.y, x.pos_b.x, x.pos_b.y);

            // malý puntík na začátku čáry
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect dot = {static_cast<int>(x.pos_a.x - 2), static_cast<int>(x.pos_a.y - 2), 4, 4};
            SDL_RenderFillRect(renderer, &dot);

            // vykreslení čísla u čáry
            if (font) {
                std::string label = std::to_string(index);
                SDL_Color textColor = {255, 255, 255, 255}; // bílá barva
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, label.c_str(), textColor);
                if (textSurface) {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_Rect dstRect = {static_cast<int>(x.pos_a.x + 4), static_cast<int>(x.pos_a.y + 4), textSurface->w, textSurface->h};
                    SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
                    SDL_DestroyTexture(textTexture);
                    SDL_FreeSurface(textSurface);
                }
            }

            index++;
        }

        if (curr->front) node_queue.push(curr->front.get());
        if (curr->back)  node_queue.push(curr->back.get());
    }
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