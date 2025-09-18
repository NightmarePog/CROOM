#include "utils/line_segment.hpp"
#include <stdexcept>


Partition::Partition(LineSegment partition) {
this->partition = partition;
}


Side Partition::decide_side(LineSegment segment) {
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


bool Partition::is_in_front(LineSegment segment) {
    auto side = [](const Vec2& p, const LineSegment& line) {
        return (line.pos_b.x - line.pos_a.x) * (p.y - line.pos_a.y) -
               (line.pos_b.y - line.pos_a.y) * (p.x - line.pos_a.x);
    };

    float d1 = side(segment.pos_a, this->partition);
    float d2 = side(segment.pos_b, this->partition);
    return d1 > 0 && d2 > 0;
}



bool Partition::is_in_back(LineSegment segment) {
    auto side = [](const Vec2& p, const LineSegment& line) {
        return (line.pos_b.x - line.pos_a.x) * (p.y - line.pos_a.y) -
               (line.pos_b.y - line.pos_a.y) * (p.x - line.pos_a.x);
    };
    float d1 = side(segment.pos_a, this->partition);
    float d2 = side(segment.pos_b, this->partition);

    return d1 < 0 && d2 < 0;
}

