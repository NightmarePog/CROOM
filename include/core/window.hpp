#pragma once
#include <SDL2/SDL.h>

class Window {
public:
    explicit Window(const char* title, int width, int height);

    ~Window();

    void run();

    void drawLine();
    
    bool repeat();

private:
    bool running;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
};
