#include "utils/vec2.hpp"
#include <optional>

// TODO
class Raycast {
    public:
    Vec2 start;
    Vec2 direction;

    Raycast(const Vec2& startPos, const Vec2& dir);

    std::optional<Vec2> intersectsSegment(const Vec2& A, const Vec2& B) const;

    void update(float dt) ;
};