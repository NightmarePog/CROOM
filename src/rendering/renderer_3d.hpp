#pragma once

#include <string_view>

struct SDL_Renderer;

namespace croom {

class Map;
struct ProjectedWallColumn;
struct Viewport;

class Renderer3D {
public:
  explicit Renderer3D(SDL_Renderer &renderer);

  void render(const Map &map);

private:
  void require_draw(bool success, std::string_view action);
  void set_color(int red, int green, int blue, std::string_view action);
  void fill_rectangle(int x, int y, int width, int height,
                      std::string_view action);
  Viewport output_viewport();
  void draw_background(Viewport viewport);
  void set_wall_color(float distance);
  void draw_wall_column(const ProjectedWallColumn &column);
  void draw_view(const Map &map, Viewport viewport);

  SDL_Renderer *renderer_;
};

}
