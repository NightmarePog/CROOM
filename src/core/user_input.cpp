 #include "core/user_input.hpp"
#include <SDL_events.h>
#include <SDL_keycode.h>

UserInputService::UserInputService() {};

UserInput UserInputService::poll_input_event() {
    while (SDL_PollEvent(&this->event)) {
        if (event.type == SDL_QUIT)
            return UserInput::QUIT;
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_w: return UserInput::W;
                case SDLK_a: return UserInput::A;
                case SDLK_s: return UserInput::S;
                case SDLK_d: return UserInput::D;
                case SDLK_ESCAPE: return UserInput::QUIT;
            }
        }
    }
    return UserInput::UNDEFINED;
}
