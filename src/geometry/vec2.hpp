#pragma once

#include <cmath>

namespace croom {

struct Vec2 {
  float x = 0.0f;
  float y = 0.0f;

  friend constexpr bool operator==(Vec2 left, Vec2 right) {
    return left.x == right.x && left.y == right.y;
  }

  friend constexpr Vec2 operator+(Vec2 left, Vec2 right) { return left += right; }

  friend constexpr Vec2 operator-(Vec2 left, Vec2 right) { return left -= right; }

  friend constexpr Vec2 operator-(Vec2 value) { return {-value.x, -value.y}; }

  friend constexpr Vec2 operator*(Vec2 value, float scalar) { return value *= scalar; }

  friend constexpr Vec2 operator*(float scalar, Vec2 value) { return value *= scalar; }

  friend constexpr Vec2 operator/(Vec2 value, float scalar) { return value /= scalar; }

  constexpr Vec2 &operator+=(Vec2 other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr Vec2 &operator-=(Vec2 other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  constexpr Vec2 &operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  constexpr Vec2 &operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  constexpr float dot(Vec2 other) const { return x * other.x + y * other.y; }

  constexpr float cross(Vec2 other) const { return x * other.y - y * other.x; }

  constexpr float length_squared() const { return dot(*this); }

  float length() const { return std::hypot(x, y); }

  bool is_finite() const { return std::isfinite(x) && std::isfinite(y); }

  Vec2 normalized_or(Vec2 fallback, float epsilon) const {
    const float magnitude = length();
    return magnitude > epsilon ? *this / magnitude : fallback;
  }
};

}
