#include "game/raycast/raycast.hpp"
#include <cmath>
#include <optional>

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

std::optional<Vec2> Raycast::intersectsSegment(const Vec2&, const Vec2&) {
    return std::nullopt;
}
