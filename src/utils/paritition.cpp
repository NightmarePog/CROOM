#include "utils/line_segment.hpp"
#include "core/bsp.hpp"
#include <iostream>
#include <stdexcept>


Partition::Partition(LineSegment partition) {
   this->partition = partition;
}


Side Partition::decide_side(LineSegment segment) {
    std::cout << "BACK:" << is_in_back(segment) << "FRONT:"<< is_in_front(segment) << std::endl;
    if (is_in_front(segment) && is_in_back(segment)) {
        return  Side::BETWEEN;
    }
    if (is_in_front(segment)) {
        return Side::FRONT;
    }
    if (is_in_back(segment)) {
        return  Side::BACK;
    }
    throw std::runtime_error("Partition::decide_side - unknown side type");
}

// MADE WRONG
bool Partition::is_in_front(LineSegment segment) {
    auto side = [](const Vec2& p, const LineSegment& line) {
        return (line.pos_b.x - line.pos_a.x) * (p.y - line.pos_a.y) -
               (line.pos_b.y - line.pos_a.y) * (p.x - line.pos_a.x);
    };

    float d1 = side(segment.pos_a, partition);
    float d2 = side(segment.pos_b, partition);

    return d1 >= 0 && d2 >= 0;
}


// MADE WRONG
bool Partition::is_in_back(LineSegment segment) {
    auto side = [](const Vec2& p, const LineSegment& line) {
        return (line.pos_b.x - line.pos_a.x) * (p.y - line.pos_a.y) -
               (line.pos_b.y - line.pos_a.y) * (p.x - line.pos_a.x);
    };

    float d1 = side(segment.pos_a, partition);
    float d2 = side(segment.pos_b, partition);

    return d1 <= 0 && d2 <= 0;
}

