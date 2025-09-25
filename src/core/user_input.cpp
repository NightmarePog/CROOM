 #include "core/user_input.hpp"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL2/SDL.h>
#include <SDL_scancode.h>
#include <cstddef>
#include <cstdlib>
#include <vector>

UserInputService::UserInputService() {};

const UserInput inputs[] = {UserInput::QUIT, UserInput::W, UserInput::A, UserInput::S, UserInput::D, UserInput::ARROW_LEFT, UserInput::ARROW_RIGHT};

std::vector<UserInput> UserInputService::poll_input_event() {
    std::vector<UserInput>output;

    while (SDL_PollEvent(&this->event)) {
        if (event.type == SDL_QUIT) {
            output = {inputs[0]};
            return output;
        }
    }

    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    const Uint8 scans[] = {keystates[SDL_SCANCODE_Q], keystates[SDL_SCANCODE_W], keystates[SDL_SCANCODE_A], keystates[SDL_SCANCODE_S], keystates[SDL_SCANCODE_D], keystates[SDL_SCANCODE_LEFT], keystates[SDL_SCANCODE_RIGHT]};

    int added = 0;
    int i = 0;
    for (int scan : scans) {
        if (scan) {
            output.resize(added+1);
            output[added] = inputs[i];
            added++;
        }
        i++;
    }
    output.resize(added+1);
    output[added] = UserInput::UNDEFINED;

    return output;
}
