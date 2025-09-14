#pragma once
#include "game/entity.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <string>
#include "utils/vec2.hpp"

constexpr int TEXTURE_SIZE = 64;

struct Sprite_Cords {
    Vec2 cord_a;
    Vec2 cord_b;
};

class Sprite : public Entity {
private: 
    SDL_Texture* texture;
    Sprite_Cords cords;

    SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);

public:
    Sprite(SDL_Renderer* renderer, std::string texture_path, Sprite_Cords cords);
    ~Sprite();

    SDL_Texture* get_texture();
    Sprite_Cords get_cords();
};
