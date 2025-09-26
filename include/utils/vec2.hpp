#pragma once

/** simple struct of vector with 2 values and overrides */
struct Vec2 {
    float x, y;

    Vec2(float x = 0, float y = 0) : x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
};
