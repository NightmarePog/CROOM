#include "core/bsp.hpp"
#include "core/graphics_window.hpp"
#include "core/user_input.hpp"
#include "game/map.hpp"
#include "game/player.hpp"
#include "utils/globals.hpp"
#include <SDL2/SDL.h>
#include <SDL_timer.h>
#include <vector>
GraphicsWindow window("CROOM", 1000, 1000);

bool running = true;

// Main loop: repeatedly calls window.tick() every `interval` seconds.
// Updates the window and processes input events.
void heartbeat(double interval) {
  std::vector<UserInput> user_input;
  while (running) {
    user_input = globals::user_input.poll_input_event();
    // quickly exits without looping all keys
    if (user_input[0] == UserInput::QUIT) {
      running = false;
    }

    globals::map.get_player()->move(user_input);
      globals::map.get_player()->rotate(user_input);
    window.tick();
    SDL_Delay(interval * 100);
  }
}

/**
function being called after everything is loaded
*/
void ready() { heartbeat(constants::FRAME_INTERVAL); }

int main() {
  // loads all needed assets
  globals::map = Map({128, 128});
  globals::map.import_from_csv("./assets/map.csv");
  globals::map.add_player(new Player(90));
  BSP bsp;
  bsp.load_from_map(&globals::map);
  bsp.build_bsp();
  // user input init
  globals::user_input = UserInputService();
  ready();
}
