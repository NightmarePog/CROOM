#include "rendering/graphics_window.hpp"

#include "rendering/renderer_2d.hpp"

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

class GraphicsWindow::RendererFactory {
public:
  explicit RendererFactory(SDL_Window &window) : window_(&window) {}

  RendererHandle create() { return create_accelerated(); }

private:
  string failure_message() const {
    return "SDL renderer creation failed (accelerated: " + accelerated_error_ +
           "; software: " + software_error_ + "; default: " + SDL_GetError() + ")";
  }

  RendererHandle try_create(Uint32 flags) {
    return RendererHandle{SDL_CreateRenderer(window_, -1, flags)};
  }

  RendererHandle create_accelerated() {
    if (auto renderer = try_create(SDL_RENDERER_ACCELERATED))
      return renderer;
    accelerated_error_ = SDL_GetError();
    return create_software();
  }

  RendererHandle create_software() {
    if (auto renderer = try_create(SDL_RENDERER_SOFTWARE))
      return renderer;
    software_error_ = SDL_GetError();
    return create_fallback();
  }

  RendererHandle create_fallback() {
    if (auto renderer = try_create(0))
      return renderer;
    throw runtime_error(failure_message());
  }

  SDL_Window *window_;
  string accelerated_error_;
  string software_error_;
};

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
      sdl_renderer_(RendererFactory{*window_}.create()), renderer_2d_(*sdl_renderer_) {}

void GraphicsWindow::present(const Map &map) {
  renderer_2d_.render(map);
  SDL_RenderPresent(sdl_renderer_.get());
}

}
