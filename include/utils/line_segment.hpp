#pragma once
#include "utils/vec2.hpp"

typedef struct {
    Vec2 pos_a;
    Vec2 pos_b;
} LineSegment;


class Partition {

    private:
        LineSegment partition;

    public:
        Partition(LineSegment partition);
        bool is_in_front(LineSegment segment);
        bool is_in_back(LineSegment segment) ;
};