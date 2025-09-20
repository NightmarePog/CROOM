#pragma once
#include "game/entity.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <string>
#include "utils/line_segment.hpp"

constexpr int TEXTURE_SIZE = 64;

/**
sprite of the game, it's a segment with properities like texture etx.
*/
class Sprite : public Entity {
private: 
    /**
    Sprite texture
    */
    SDL_Texture* texture;
    /**
    Sprite position
    */
    LineSegment line;
    /**
    loads a texture into class property
    @returns SDL_Texture* 
    */
    SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);

public:
    Sprite(SDL_Renderer* renderer, std::string texture_path, LineSegment cords);
    ~Sprite();

    /**
    gets sprites texture
    @returns SDL_Texture*
    */
    SDL_Texture* get_texture();

    /**
    gets sprites position
    @returns LineSegment
    */
    LineSegment get_cords();
};
