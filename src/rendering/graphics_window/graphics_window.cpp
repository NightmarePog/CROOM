#include "rendering/graphics_window/graphics_window.hpp"
#include "game/bsp/bsp.hpp"
#include "game/map/map.hpp"
#include "rendering/renderer_2d/renderer_2d.hpp"

#include <SDL2/SDL.h>
#include <SDL_render.h>

#include <stdexcept>

GraphicsWindow::GraphicsWindow(const char *title, int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error(SDL_GetError());
  }
  window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN);

  if (!window) {
    SDL_Quit();
    throw std::runtime_error(SDL_GetError());
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_DestroyWindow(window);
    SDL_Quit();
    throw std::runtime_error(SDL_GetError());
  }
}

GraphicsWindow::~GraphicsWindow() {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
  SDL_Quit();
}

SDL_Renderer *GraphicsWindow::get_renderer() const { return renderer; }

void GraphicsWindow::tick(Map &map, BSPNode *bsp_node) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
  SDL_RenderClear(renderer);
  render(renderer, &map);
  render_lines(renderer, bsp_node);
  SDL_RenderPresent(renderer);
}
