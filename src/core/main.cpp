#include <SDL2/SDL.h>
#include <SDL_timer.h>
#include "core/2d_render.hpp"
#include "core/graphics_window.hpp"
#include "game/entity.hpp"
#include "game/map.hpp"
#include "game/sprite.hpp"
#include "utils/const.hpp"
#include "core/2d_render.hpp"


GraphicsWindow window("a window", 1000, 1000);
Map map = Map({128, 128});


bool running = true;
void heartbeat(double interval) {
   while (running) {
   running = window.tick();
   render(window.get_renderer(), &map);
   SDL_Delay(interval*1000);
   }
}

int main() {
   map.add_entity(
      new Sprite(window.get_renderer(), "A test", {20,30}, {80,10})

   );
   heartbeat(FRAME_INTERVAL);
}
