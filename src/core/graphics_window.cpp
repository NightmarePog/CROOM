#include "core/graphics_window.hpp"
#include <SDL2/SDL.h>
#include <SDL_keycode.h>
#include <stdexcept>

GraphicsWindow::GraphicsWindow(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }
}

GraphicsWindow::~GraphicsWindow() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}


bool GraphicsWindow::tick() {
    // TODO for now i am just testing with texturing etc. :3, so this is not really finished
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect srcRect = {0, 0, 64, 64};
    SDL_Rect dst     = {100, 100, 64, 64};
    double angle     = 0.0;
    SDL_Point center = {dst.w / 2, dst.h / 2};
    //SDL_Texture* texture = load; 

    SDL_RenderCopyEx(renderer, texture, &srcRect, &dst, angle, &center, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
    return is_quit_pressed();
}
bool GraphicsWindow::is_quit_pressed() {
    while (SDL_PollEvent(&event)) {
        return !( event.type == SDL_KEYDOWN && (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE));
    }
    return true;
}