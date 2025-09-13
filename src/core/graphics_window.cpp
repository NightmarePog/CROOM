#include "core/graphics_window.hpp"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return is_quit_pressed();
}

bool GraphicsWindow::is_quit_pressed() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            return false;
    }
    return true; // won't quit
}


SDL_Renderer* GraphicsWindow::get_renderer() {
    return renderer;
}