#include "rendering/graphics_window.hpp"

#include "rendering/renderer_3d.hpp"
#include "rendering/sdl_renderer_factory.hpp"

#include <SDL.h>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std;

namespace croom {
namespace {
constexpr int centered = SDL_WINDOWPOS_CENTERED;
constexpr Uint32 shown = SDL_WINDOW_SHOWN;
}

void GraphicsWindow::WindowDeleter::operator()(SDL_Window *window) {
  SDL_DestroyWindow(window);
}

void GraphicsWindow::RendererDeleter::operator()(SDL_Renderer *renderer) {
  SDL_DestroyRenderer(renderer);
}

GraphicsWindow::WindowHandle GraphicsWindow::create_window(const char *title, int width,
                                                           int height) {
  return require_window(
      WindowHandle{SDL_CreateWindow(title, centered, centered, width, height, shown)});
}

GraphicsWindow::WindowHandle GraphicsWindow::require_window(WindowHandle window) {
  if (!window)
    throw runtime_error("SDL window creation failed: " + string{SDL_GetError()});
  return window;
}

GraphicsWindow::GraphicsWindow(const char *title, int width, int height)
    : window_(create_window(title, width, height)),
      sdl_renderer_(SdlRendererFactory{*window_}.create()),
      renderer_3d_(*sdl_renderer_) {}

void GraphicsWindow::present(const Map &map) {
  renderer_3d_.render(map);
  SDL_RenderPresent(sdl_renderer_.get());
}

}
