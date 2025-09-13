#pragma once
#include "game/sprite.hpp"
#include <SDL2/SDL.h>
#include <SDL_render.h>

class GraphicsWindow {
public:
    explicit GraphicsWindow(const char* title, int width, int height);

    ~GraphicsWindow();
    
    void drawLine();
    
    bool tick();

    SDL_Renderer* get_renderer();

private:
    bool is_quit_pressed();
    bool running;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Sprite* test_sprite;
};
