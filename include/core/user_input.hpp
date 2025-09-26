#pragma once
#include <SDL_events.h>
#include <queue>

/**
enum of available keys
QUIT = esc
*/
enum class UserInput {
    W,
    A,
    S,
    D,
    ARROW_LEFT,
    ARROW_RIGHT,
    QUIT,
    UNDEFINED
};

class UserInputService {
    private:
        SDL_Event event;
    public:
        UserInputService();
        /**
        gets a key that user is currectly holding
        @returns a key player is currectly holding
        */
        std::queue<UserInput> poll_input_event();

    
};