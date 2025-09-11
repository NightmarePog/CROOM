#pragma once
#include <SDL2/SDL.h>

class Window {
public:
    explicit Window(const char* title, int width, int height);

    ~Window();

    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};
