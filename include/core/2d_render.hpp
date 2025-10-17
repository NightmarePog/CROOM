#pragma once
#include "core/bsp.hpp"
#include "game/map.hpp"
#include <SDL_render.h>
#include <SDL_ttf.h>


/** 
   renders the 2D scene
   @param *renderer - a window rendern
   @param *map - map to render
*/
void render(SDL_Renderer *renderer, Map *map);


void render_lines(SDL_Renderer *renderer, BSPNode *bsp, TTF_Font* font);