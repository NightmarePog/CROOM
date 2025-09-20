#pragma once

#include "game/map.hpp"
#include "core/user_input.hpp"
#include <SDL_render.h>

/** 
constant values of the program, these values stay static no matter what
 */
namespace constants {
    /**
    interval between frames
    */
    constexpr double FRAME_INTERVAL = 1.0/60.0;
};


namespace globals {
    /**
    map instance
    */
    inline Map map({0, 0});
    /**
    renderer pointer
    */
    inline SDL_Renderer *renderer;
    /**
    UserInputService instance
    */
    inline UserInputService user_input;
}
