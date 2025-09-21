 #include "core/user_input.hpp"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL2/SDL.h>
#include <SDL_scancode.h>

// TODO multiple key press
// IDEA might return queue of keys

UserInputService::UserInputService() {};


UserInput UserInputService::poll_input_event() {
    while (SDL_PollEvent(&this->event)) {
        if (event.type == SDL_QUIT)
            return UserInput::QUIT;
    }

    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_W]) return UserInput::W;
    if (keystates[SDL_SCANCODE_A]) return UserInput::A;
    if (keystates[SDL_SCANCODE_S]) return UserInput::S;
    if (keystates[SDL_SCANCODE_D]) return UserInput::D;
    if (keystates[SDL_SCANCODE_LEFT]) return UserInput::ARROW_LEFT;
    if (keystates[SDL_SCANCODE_RIGHT]) return UserInput::ARROW_RIGHT;
    if (keystates[SDL_SCANCODE_ESCAPE]) return UserInput::QUIT;

    return UserInput::UNDEFINED;
}
