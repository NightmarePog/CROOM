#pragma once
#include "game/map.hpp"
#include <SDL_render.h>


/** 
   renders the 2D scene
   @param *renderer - a window rendern
   @param *map - map to render
*/
void render(SDL_Renderer *renderer, Map *map);
