#pragma once
#include <SDL2/SDL.h>

class GraphicsWindow {
public:
    explicit GraphicsWindow(const char* title, int width, int height);

    ~GraphicsWindow();
    
    void drawLine();
    
    bool tick();

private:

    bool is_quit_pressed();
    bool running;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
};
