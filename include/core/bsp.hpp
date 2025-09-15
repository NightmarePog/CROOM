#pragma  once

#include "game/entity.hpp"
#include "game/map.hpp"
#include "game/sprite.hpp"
#include "utils/line_segment.hpp"
#include "utils/vec2.hpp"
#include <queue>
#include <vector>




enum class Leaf {
    SOLID = -1,
    EMPTY = -2,
};


struct BSPNode {
    BSPNode *front;
    BSPNode *back;
    Leaf leaf;
    LineSegment partition_segment;
    std::vector<LineSegment> node_segments;
};


class BSP {
    private: 
        BSPNode currect_node;
        std::queue<BSPNode> node_queue;
        int level;
        bool is_bsp;
        bool is_loaded = false;

        LineSegment get_longest_segment(std::vector<LineSegment> segments);
    public:
        BSP();
        void load_from_csv();
        void load_from_map(Map *map);
        void create_bsp();
};

