#pragma once

#include "game/map.hpp"
#include "core/user_input.hpp"
#include <SDL_render.h>
namespace constants {
constexpr double FRAME_INTERVAL = 1.0/60.0;
};


namespace globals {
    inline Map map({0, 0});
    inline SDL_Renderer *renderer;
    inline UserInputService user_input;
}
