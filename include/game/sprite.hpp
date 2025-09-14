#pragma once
#include "game/entity.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <string>
#include "utils/line_segment.hpp"

constexpr int TEXTURE_SIZE = 64;

class Sprite : public Entity {
private: 
    SDL_Texture* texture;
    LineSegment line;

    SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);

public:
    Sprite(SDL_Renderer* renderer, std::string texture_path, LineSegment cords);
    ~Sprite();

    SDL_Texture* get_texture();
    LineSegment get_cords();
};
