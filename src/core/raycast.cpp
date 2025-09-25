#include "core/bsp.hpp"
#include "core/graphics_window.hpp"
#include "core/user_input.hpp"
#include "game/map.hpp"
#include "game/player.hpp"
#include "game/sprite.hpp"
#include "utils/globals.hpp"

#include "utils/vec2.hpp"
#include <cmath>
#include <optional>
#include <vector>
#include "core/raycast.hpp"

Raycast::Raycast(Map *map, const Vec2& startPos, const Vec2& endPos) {
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