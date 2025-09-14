#include "game/sprite.hpp"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <iostream>

Sprite::Sprite(SDL_Renderer *renderer, std::string texture_path,
               LineSegment line) {
  if (!renderer) {
    std::cerr << "Sprite: renderer is null" << std::endl;
    return;
  }
  // process texture
  SDL_Color texture_color = {255, 255, 0, 255};
  this->texture = this->load_texture(renderer, texture_path.c_str());
  if (this->texture) {
    texture_color = {255, 255, 255, 255};
  }

  this->line = line;
}

Sprite::~Sprite() {}

SDL_Texture *Sprite::load_texture(SDL_Renderer *renderer,
                                  const char *filepath) {
  SDL_Surface *loadedSurface = SDL_LoadBMP(filepath);
  if (!loadedSurface)
    return nullptr;

  SDL_Surface *scaledSurface =
      SDL_CreateRGBSurface(0, TEXTURE_SIZE, TEXTURE_SIZE, 32, 0, 0, 0, 0);
  if (!scaledSurface) {
    SDL_FreeSurface(loadedSurface);
    return nullptr;
  }

  SDL_Rect srcRect = {0, 0, loadedSurface->w, loadedSurface->h};
  SDL_Rect destRect = {0, 0, TEXTURE_SIZE, TEXTURE_SIZE};
  SDL_BlitScaled(loadedSurface, &srcRect, scaledSurface, &destRect);

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, scaledSurface);

  SDL_FreeSurface(loadedSurface);
  SDL_FreeSurface(scaledSurface);

  return texture;
}

SDL_Texture *Sprite::get_texture() { return texture; }

LineSegment Sprite::get_cords() { return line; }
