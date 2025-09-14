#include "core/services/2d_render.hpp"
#include "game/map.hpp"
#include "game/sprite.hpp"
#include <SDL_render.h>



void render(SDL_Renderer *renderer, Map *map) {
    if (!renderer) return;
    const auto& sprite_vec = map->get_entity_vector(); // reference, ne kopie
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (size_t i = 0; i < sprite_vec.size(); ++i) {     
        Sprite_Cords cords = map->get_entity_vector()[i]->get_cords();
        SDL_RenderDrawLine(renderer, cords.cord_a.x, cords.cord_a.y, cords.cord_b.x, cords.cord_b.y);
    }
};

