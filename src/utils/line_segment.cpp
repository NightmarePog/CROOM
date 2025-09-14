#include "utils/line_segment.hpp"


Partition::Partition(LineSegment partition) {
   this->partition = partition;
}



bool Partition::is_in_front(LineSegment segment) {
    auto side = [](const Vec2& p, const LineSegment& line) {
        return (line.pos_b.x - line.pos_a.x) * (p.y - line.pos_a.y) -
               (line.pos_b.y - line.pos_a.y) * (p.x - line.pos_a.x);
    };

    float d1 = side(segment.pos_a, partition);
    float d2 = side(segment.pos_b, partition);

    return d1 >= 0 && d2 >= 0;
}

bool Partition::is_in_back(LineSegment segment) {
    auto side = [](const Vec2& p, const LineSegment& line) {
        return (line.pos_b.x - line.pos_a.x) * (p.y - line.pos_a.y) -
               (line.pos_b.y - line.pos_a.y) * (p.x - line.pos_a.x);
    };

    float d1 = side(segment.pos_a, partition);
    float d2 = side(segment.pos_b, partition);

    return d1 <= 0 && d2 <= 0;
}

