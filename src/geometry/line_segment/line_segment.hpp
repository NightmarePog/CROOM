#pragma once
#include "geometry/vec2/vec2.hpp"

/**
enum of sides+BETWEEN
*/
enum class Side {
    FRONT,
    BACK,
    BETWEEN,
};

/**
line segment struct made of 2 points
 */
typedef struct {
    Vec2 pos_a;
    Vec2 pos_b;
} LineSegment;
