#pragma  once

#include "game/map.hpp"
#include "utils/line_segment.hpp"
#include <memory>
#include <optional>
#include <vector>



enum class Leaf {
    SOLID = -1,
    EMPTY = -2,
};


struct BSPNode {
    std::unique_ptr<BSPNode> front;
    std::unique_ptr<BSPNode> back;
    std::optional<Partition> partition;
    std::vector<LineSegment> segments;
};


class BSP {
    private: 
        bool is_loaded;
        BSPNode root;
        LineSegment get_longest_segment(std::vector<LineSegment> segments);
        void process_node(BSPNode node);
    public:
        BSP();
        void load_from_csv();
        void load_from_map(Map *map);
        void build_bsp();
};

