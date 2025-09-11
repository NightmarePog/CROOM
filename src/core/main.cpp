#include <SDL2/SDL.h>
#include "core/window.hpp"

int main() {
   Window window("a window", 1000, 1000);
   window.run();
}
