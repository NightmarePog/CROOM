#pragma once
#include <SDL2/SDL.h>

class Map;
struct BSPNode;

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
    void tick(Map &map, BSPNode *bsp_node);

    SDL_Renderer *get_renderer() const;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};
