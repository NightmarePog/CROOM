#include <SDL2/SDL.h>
#include <SDL_timer.h>
#include <cstdio>
#include "core/window.hpp"
#include "utils/const.hpp"

Window window("a window", 1000, 1000);
bool running = true;
void heartbeat(double interval) {
   while (running) {
   running = window.repeat();
   SDL_Delay(interval*1000);
   }
}

int main() {
   window.run();
   heartbeat(FRAME_INTERVAL);


}
