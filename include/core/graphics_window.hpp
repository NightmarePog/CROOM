#pragma once
#include "game/sprite.hpp"
#include <SDL2/SDL.h>
#include <SDL_render.h>

class GraphicsWindow {
public:
    /**
    creates Graphics window
    */
    explicit GraphicsWindow(const char* title, int width, int height);

    ~GraphicsWindow();
    /**
    runs on game tick, rerender the scene for next frame
    */    
    void tick();

private:
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Sprite* test_sprite;
};
