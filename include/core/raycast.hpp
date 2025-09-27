#include "game/map.hpp"
#include "utils/vec2.hpp"
#include <optional>

class Raycast {
    private:
    /*
    defines where to raycast 
    */
    Map *map;
    /*
    defines where the raycast starts
    */
    Vec2 start;
    /*
    defines where the raycast ends
    */
    Vec2 end;
    /*
    defines where the raycast points
    */
    Vec2 dir;
    /*
    defines how long the raycast is
    */
    float distance;

    public:
    /*
    creates a raycast
    */
    Raycast(Map *map, const Vec2& startPos, const Vec2& endPos);
    /*
    returns position of intersected segment
    */
    std::optional<Vec2> intersectsSegment(const Vec2& A, const Vec2& B);
};