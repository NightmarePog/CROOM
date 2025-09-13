#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("IMG_Load failed: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
    }

    return texture;
}
