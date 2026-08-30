#pragma once

#include "rendering/renderer_2d.hpp"

#include <memory>

struct SDL_Renderer;
struct SDL_Window;

namespace croom {

class Map;

class GraphicsWindow {
public:
  GraphicsWindow(const char *title, int width, int height);
  ~GraphicsWindow() = default;

  GraphicsWindow(const GraphicsWindow &) = delete;
  GraphicsWindow &operator=(const GraphicsWindow &) = delete;
  GraphicsWindow(GraphicsWindow &&) = delete;
  GraphicsWindow &operator=(GraphicsWindow &&) = delete;

  void present(const Map &map);

private:
  class WindowDeleter {
  public:
    void operator()(SDL_Window *window);
  };

  class RendererDeleter {
  public:
    void operator()(SDL_Renderer *renderer);
  };

  using WindowHandle = std::unique_ptr<SDL_Window, WindowDeleter>;
  using RendererHandle = std::unique_ptr<SDL_Renderer, RendererDeleter>;

  class RendererFactory;

  static WindowHandle create_window(const char *title, int width, int height);
  static WindowHandle require_window(WindowHandle window);

  WindowHandle window_;
  RendererHandle sdl_renderer_;
  Renderer2D renderer_2d_;
};

}
