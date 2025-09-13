
#pragma once
#include "game/entity.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <string>
#include "utils/vec2.hpp"
constexpr int TEXTURE_SIZE = 64;

typedef struct {
    Vec2 cord_a;
    Vec2 cord_b;
} Sprite_Cords;
class Sprite : public Entity {
    private: 
        SDL_Texture* texture;
        SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);
        Sprite_Cords cords;
    public:

        Sprite(SDL_Renderer* renderer, std::string texture_path, Vec2 position_a, Vec2 position_b);

        ~Sprite();

        SDL_Texture* get_texture();

        Sprite_Cords get_cords();


};