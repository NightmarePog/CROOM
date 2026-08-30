#include "rendering/renderer_2d.hpp"

#include "game/map.hpp"
#include "game/player.hpp"
#include "geometry/line_segment.hpp"

#include <SDL.h>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std;

namespace croom {
namespace {
constexpr float direction_length = Player::collision_radius * 2.0f;
constexpr string_view player_body_action = "drawing the player body failed";
constexpr string_view clear_color_action = "setting the clear color failed";
constexpr string_view scene_color_action = "setting the scene color failed";
}

Renderer2D::Renderer2D(SDL_Renderer &renderer) : renderer_(&renderer) {}

void Renderer2D::require_draw(bool success, string_view action) {
  if (!success)
    throw runtime_error(string{action} + ": " + SDL_GetError());
}

void Renderer2D::set_color(uint8_t red, uint8_t green, uint8_t blue,
                           string_view action) {
  require_draw(SDL_SetRenderDrawColor(renderer_, red, green, blue, 255) == 0, action);
}

void Renderer2D::clear() {
  set_color(0, 0, 50, clear_color_action);
  require_draw(SDL_RenderClear(renderer_) == 0, "clearing the renderer failed");
}

void Renderer2D::set_scene_color() { set_color(255, 0, 0, scene_color_action); }

void Renderer2D::draw_line(Vec2 start, Vec2 end, string_view action) {
  require_draw(SDL_RenderDrawLineF(renderer_, start.x, start.y, end.x, end.y) == 0,
               action);
}

void Renderer2D::draw_wall(const LineSegment &wall) {
  draw_line(wall.start, wall.end, "drawing a wall failed");
}

void Renderer2D::draw_walls(const Map &map) {
  set_scene_color();
  for (const LineSegment &wall : map.walls())
    draw_wall(wall);
}

void Renderer2D::draw_circle_row(int y, Vec2 center, float radius) {
  const float offset = static_cast<float>(y);
  const float width = sqrt(max(0.0f, radius * radius - offset * offset));
  const Vec2 start{center.x - width, center.y + offset};
  draw_line(start, {center.x + width, start.y}, player_body_action);
}

void Renderer2D::draw_filled_circle(Vec2 center, float radius) {
  const int min_y = static_cast<int>(floor(-radius));
  const int max_y = static_cast<int>(ceil(radius));
  for (int y = min_y; y <= max_y; ++y)
    draw_circle_row(y, center, radius);
}

Vec2 Renderer2D::direction_end(const Player &player) {
  const Vec2 position = player.position();
  const float radians = player.heading_degrees() * (numbers::pi_v<float> / 180.0f);
  return {position.x + sin(radians) * direction_length,
          position.y - cos(radians) * direction_length};
}

void Renderer2D::draw_direction(const Player &player) {
  draw_line(player.position(), direction_end(player),
            "drawing the player direction failed");
}

void Renderer2D::draw_player(const Player &player) {
  set_scene_color();
  draw_filled_circle(player.position(), Player::collision_radius);
  draw_direction(player);
}

void Renderer2D::render(const Map &map) {
  clear();
  draw_walls(map);
  draw_player(map.player());
}

}
