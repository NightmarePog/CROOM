#include "core/bsp.hpp"
#include "game/map.hpp"

#include "utils/vec2.hpp"
#include <cmath>
#include <optional>
#include "core/raycast.hpp"

// TODO implement raycast
// Basic idea of how the structure should look?

Raycast::Raycast(Map *map, const Vec2& startPos, const Vec2& endPos) {
    // Helpful variables
    this->map = map;
    this->start = startPos;
    this->end = endPos;
    this->dir = endPos-startPos;
    this->distance = sqrt(pow(dir.x,2) + pow(dir.y,2));
}

std::optional<Vec2> Raycast::intersectsSegment(const Vec2& A, const Vec2& B) {
    // dir + startPos = endPos
    for (int i = 0; i < this->distance; i++) {
        // TODO
        // check if intersecting
    }
}