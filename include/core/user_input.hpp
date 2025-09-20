#pragma once
#include <SDL_events.h>
enum class UserInput {
    W,
    A,
    S,
    D,
    QUIT,
    UNDEFINED
};

class UserInputService {
    private:
        SDL_Event event;
    public:
        UserInputService();
        UserInput poll_input_event();

    
};