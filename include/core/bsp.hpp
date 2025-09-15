#pragma  once

#include "game/entity.hpp"
#include "game/map.hpp"
#include "game/sprite.hpp"
#include "utils/line_segment.hpp"
#include "utils/vec2.hpp"
#include <algorithm>
#include <memory>
#include <queue>
#include <vector>




enum class Leaf {
    SOLID = -1,
    EMPTY = -2,
};


struct BSPNode {
    std::unique_ptr<BSPNode> front;
    std::unique_ptr<BSPNode> back;
    LineSegment splitter;
    std::vector<LineSegment> segments;
};


class BSP {
    private: 
        bool is_loaded;
        BSPNode root;
        LineSegment get_longest_segment(std::vector<LineSegment> segments);
    public:
        BSP();
        void load_from_csv();
        void load_from_map(Map *map);
        void build_bsp();
};

