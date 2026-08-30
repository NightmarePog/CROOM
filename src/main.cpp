#include "game/bsp/bsp.hpp"
#include "game/map/map.hpp"
#include "game/player/player.hpp"
#include "input/user_input/user_input.hpp"
#include "rendering/graphics_window/graphics_window.hpp"
#include <SDL2/SDL.h>
#include <SDL_timer.h>
#include <queue>

namespace {
constexpr double FRAME_INTERVAL = 1.0 / 60.0;
}

// Main loop: repeatedly calls window.tick() every `interval` seconds.
// Updates the window and processes input events.
void heartbeat(GraphicsWindow &window, Map &map, UserInputService &input_service,
               BSPNode *bsp_node, double interval) {
  std::queue<UserInput> user_input;
  bool running = true;
  while (running) {
    user_input = input_service.poll_input_event();
    // quickly exits without looping all keys
    if (user_input.front() == UserInput::QUIT) {
      running = false;
    }

    map.get_player()->move(user_input, interval);
    map.get_player()->rotate(user_input, interval);

    window.tick(map, bsp_node);
    SDL_Delay(static_cast<Uint32>(interval * 1000));
  }
}

int main() {
  GraphicsWindow window("CROOM", 1000, 1000);
  // loads all needed assets
  Map map({128, 128});
  map.import_from_csv(window.get_renderer(), "../assets/map.csv");
  map.add_player(new Player(90));
  BSP bsp;
  bsp.load_from_map(&map);
  bsp.build_bsp();
  // user input init
  UserInputService input_service;
  heartbeat(window, map, input_service, bsp.root, FRAME_INTERVAL);
}
