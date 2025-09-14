#include <SDL2/SDL.h>
#include <SDL_timer.h>
#include "core/services/graphics_window.hpp"
#include "game/map.hpp"
#include "utils/const.hpp"
#include "utils/globals.hpp"


GraphicsWindow window("a window", 1000, 1000);


bool running = true;

// Main loop: repeatedly calls window.tick() every `interval` seconds.
// Updates the window and processes input events.
void heartbeat(double interval) {
   while (running) {
   running = window.tick();
   SDL_Delay(interval*1000);
   }
}

/**
function being called after everything is loadedd
*/
void ready() {
   heartbeat(constants::FRAME_INTERVAL);
}

int main() {
   // loads all needed assets
   globals::map = Map({128, 128});
   globals::map.import_from_csv("../assets/map.csv");

   ready();
}
