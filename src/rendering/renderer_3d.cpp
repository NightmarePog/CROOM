#include "rendering/renderer_3d.hpp"

#include "game/map.hpp"
#include "game/player.hpp"
#include "rendering/raycast_projection.hpp"

#include <SDL.h>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std;

namespace croom {
namespace {
constexpr string_view background_color_action = "setting a background color failed";
constexpr string_view wall_color_action = "setting a wall color failed";
}

Renderer3D::Renderer3D(SDL_Renderer &renderer) : renderer_(&renderer) {}

void Renderer3D::require_draw(bool success, string_view action) {
  if (!success)
    throw runtime_error(string{action} + ": " + SDL_GetError());
}

void Renderer3D::set_color(int red, int green, int blue, string_view action) {
  const auto channel = [](int value) { return static_cast<Uint8>(clamp(value, 0, 255)); };
  require_draw(SDL_SetRenderDrawColor(renderer_, channel(red), channel(green),
                                     channel(blue), 255) == 0,
               action);
}

void Renderer3D::fill_rectangle(int x, int y, int width, int height,
                                string_view action) {
  const SDL_Rect rectangle{x, y, width, height};
  require_draw(SDL_RenderFillRect(renderer_, &rectangle) == 0, action);
}

Viewport Renderer3D::output_viewport() {
  Viewport viewport{};
  require_draw(SDL_GetRendererOutputSize(renderer_, &viewport.width,
                                         &viewport.height) == 0,
               "querying the renderer size failed");
  return viewport;
}

void Renderer3D::draw_background(Viewport viewport) {
  set_color(17, 20, 38, background_color_action);
  fill_rectangle(0, 0, viewport.width, viewport.height / 2,
                 "drawing the ceiling failed");

  set_color(35, 31, 34, background_color_action);
  fill_rectangle(0, viewport.height / 2, viewport.width,
                 viewport.height - viewport.height / 2,
                 "drawing the floor failed");
}

void Renderer3D::set_wall_color(float distance) {
  const float light = clamp(1.0f - distance / 700.0f, 0.2f, 1.0f);
  set_color(static_cast<int>(225.0f * light), static_cast<int>(62.0f * light),
            static_cast<int>(52.0f * light), wall_color_action);
}

void Renderer3D::draw_wall_column(const ProjectedWallColumn &column) {
  set_wall_color(column.distance);
  require_draw(SDL_RenderDrawLine(renderer_, column.x, column.top, column.x,
                                 column.bottom) == 0,
               "drawing a raycast wall column failed");
}

void Renderer3D::draw_view(const Map &map, Viewport viewport) {
  if (viewport.width <= 0 || viewport.height <= 0)
    return;

  const RaycastCamera camera{map.player().position(),
                             map.player().heading_degrees()};
  const RaycastProjection projection{camera, viewport};
  for (const ProjectedWallColumn &column : projection.project(map.walls()))
    draw_wall_column(column);
}

void Renderer3D::render(const Map &map) {
  const Viewport viewport = output_viewport();
  draw_background(viewport);
  draw_view(map, viewport);
}

}
