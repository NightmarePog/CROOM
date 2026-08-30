#pragma once

#include <cstdint>
#include <string_view>

struct SDL_Renderer;

namespace croom {

struct LineSegment;
class Map;
class Player;
struct Vec2;

class Renderer2D {
public:
  explicit Renderer2D(SDL_Renderer &renderer);

  void render(const Map &map);

private:
  void require_draw(bool success, std::string_view action);
  void set_color(std::uint8_t red, std::uint8_t green, std::uint8_t blue,
                 std::string_view action);
  void clear();
  void set_scene_color();
  void draw_line(Vec2 start, Vec2 end, std::string_view action);
  void draw_wall(const LineSegment &wall);
  void draw_walls(const Map &map);
  void draw_circle_row(int y, Vec2 center, float radius);
  void draw_filled_circle(Vec2 center, float radius);
  static Vec2 direction_end(const Player &player);
  void draw_direction(const Player &player);
  void draw_player(const Player &player);

  SDL_Renderer *renderer_;
};

}
