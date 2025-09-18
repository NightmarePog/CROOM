#include "utils/line_segment.hpp"
#include <iostream>
#include <stdexcept>


Partition::Partition(LineSegment partition) {
    std::cout << "CREATING PARTITION:" << partition.pos_a.x << "," << partition.pos_a.y << " " << partition.pos_b.x << "," << partition.pos_b.y << std::endl;
   this->partition = partition;
}


Side Partition::decide_side(LineSegment segment) {
    std::cout << "BACK:" << is_in_back(segment) << "FRONT:"<< is_in_front(segment) << std::endl;
    if (is_in_front(segment)) {
        return Side::FRONT;
    }
    if (is_in_back(segment)) {
        return  Side::BACK;
    }
    return Side::BETWEEN;
    char buf[256];
    std::sprintf(buf, "Partition::decide_side - unknown side type\n IS IN FRONT: %d\n IS IN BACK: %d\n", is_in_front(segment), is_in_back(segment));
    throw std::runtime_error(buf);

}

// MADE WRONG
bool Partition::is_in_front(LineSegment segment) {
    auto side = [](const Vec2& p, const LineSegment& line) {
        return (line.pos_b.x - line.pos_a.x) * (p.y - line.pos_a.y) -
               (line.pos_b.y - line.pos_a.y) * (p.x - line.pos_a.x);
    };

    float d1 = side(segment.pos_a, this->partition);
    float d2 = side(segment.pos_b, this->partition);
    std::cout << "D1 and D2: " << d1 << d2 << std::endl;
    return d1 > 0 && d2 > 0;
}


// MADE WRONG
bool Partition::is_in_back(LineSegment segment) {
    auto side = [](const Vec2& p, const LineSegment& line) {
        return (line.pos_b.x - line.pos_a.x) * (p.y - line.pos_a.y) -
               (line.pos_b.y - line.pos_a.y) * (p.x - line.pos_a.x);
    };
    std::cout << "SEGMENT POSITION:" <<segment.pos_a.x << "," << segment.pos_a.y << std::endl;
    std::cout << "PARTITION POSITION:" << partition.pos_a.x << "," << partition.pos_a.y << std::endl;
    float d1 = side(segment.pos_a, this->partition);
    float d2 = side(segment.pos_b, this->partition);

    return d1 < 0 && d2 < 0;
}

