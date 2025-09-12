#include <SDL2/SDL.h>
#include <SDL_timer.h>
#include "core/graphics_window.hpp"
#include "utils/const.hpp"

GraphicsWindow window("a window", 1000, 1000);
bool running = true;
void heartbeat(double interval) {
   while (running) {
   running = window.tick();
   SDL_Delay(interval*1000);
   }
}

int main() {
   heartbeat(FRAME_INTERVAL);
}
