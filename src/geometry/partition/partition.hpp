#pragma once

#include "geometry/line_segment/line_segment.hpp"

class Partition {
public:
    explicit Partition(LineSegment partition);

    Side decide_side(LineSegment segment);
    bool is_in_front(LineSegment segment);
    bool is_in_back(LineSegment segment);

private:
    LineSegment partition;
};
