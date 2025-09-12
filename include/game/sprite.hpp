/**
 * @class Sprite
 * @brief Represents a 2D sprite entity with a texture and source/destination rectangles.
 *
 * Inherits from Entity. Manages an SDL_Texture pointer and
 * SDL_Rect structures for source (texture coordinates) and destination (screen coordinates).
 */
#pragma once
#include "game/entity.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>
class Sprite : public Entity {
public:
    /**
     * @brief Default constructor. Initializes texture to nullptr and rects to zero.
     */
    Sprite();

    /**
     * @brief Constructs a Sprite with a texture and specific source and destination rectangles.
     * @param tex Pointer to the SDL_Texture.
     * @param src Source rectangle defining which part of the texture to use.
     * @param dst Destination rectangle defining where to render the sprite on screen.
     */
    Sprite(SDL_Texture* tex, const SDL_Rect& src, const SDL_Rect& dst);

    /**
     * @brief Destructor. Does not free the texture.
     */
    ~Sprite();

    /**
     * @brief Sets the texture pointer.
     * @param tex Pointer to the SDL_Texture.
     */
    void setTexture(SDL_Texture* tex);

    /**
     * @brief Sets the source rectangle.
     * @param rect Source rectangle defining the texture area to render.
     */
    void setSrcRect(const SDL_Rect& rect);

    /**
     * @brief Sets the destination rectangle.
     * @param rect Destination rectangle defining the screen area to render.
     */
    void setDstRect(const SDL_Rect& rect);

    /**
     * @brief Returns the texture pointer.
     * @return SDL_Texture* pointer.
     */
    SDL_Texture* getTexture() const;

    /**
     * @brief Returns the source rectangle.
     * @return SDL_Rect of the texture area.
     */
    SDL_Rect getSrcRect() const;

    /**
     * @brief Returns the destination rectangle.
     * @return SDL_Rect of the screen area.
     */
    SDL_Rect getDstRect() const;

private:
    SDL_Texture* texture; /**< Pointer to the SDL texture */
    SDL_Rect src;         /**< Source rectangle of the texture */
    SDL_Rect dst;         /**< Destination rectangle on the screen */
};
