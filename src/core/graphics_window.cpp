#include "core/graphics_window.hpp"
#include "core/2d_render.hpp"
#include "core/bsp.hpp"
#include "utils/globals.hpp"

#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_keycode.h>
#include <SDL_render.h>

#include <iostream>
#include <stdexcept>

GraphicsWindow::GraphicsWindow(const char *title, int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
   if (TTF_Init() != 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }


  window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN);

  if (!window) {
    SDL_Quit();
    throw std::runtime_error(SDL_GetError());
  }

  globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!globals::renderer) {
    SDL_DestroyWindow(window);
    SDL_Quit();
    throw std::runtime_error(SDL_GetError());
  }
}

GraphicsWindow::~GraphicsWindow() {
  if (globals::renderer)
    SDL_DestroyRenderer(globals::renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}

void GraphicsWindow::tick(BSPNode* bsp_node) {

  SDL_SetRenderDrawColor(globals::renderer, 0, 0, 50, 255);
  SDL_RenderClear(globals::renderer);
  render(globals::renderer, &globals::map);
  TTF_Font* font = TTF_OpenFont("/usr/share/fonts/TTF/Arial.TTF", 16); // nebo DejaVuSans.ttf
if (!font) {
    std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
}
  render_lines(globals::renderer, bsp_node, font);
  SDL_RenderPresent(globals::renderer);
}
