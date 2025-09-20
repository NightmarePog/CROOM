#pragma once
#include "utils/vec2.hpp"

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


/**
instance that it basically just LineSegment but can tell you what is on front and what is on back
*/
class Partition {

    private:
        /**
        partition of the space line
        */
        LineSegment partition;

    public:
        Partition(LineSegment partition);
        /**
        decide which side is it and @returns Side
        */
        Side decide_side(LineSegment segment);
        /** @returns true if is @param segment in front */
        bool is_in_front(LineSegment segment);
        /** @returns true if is @param segment in back */
        bool is_in_back(LineSegment segment) ;
};