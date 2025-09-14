#pragma  once

#include "game/map.hpp"
#include <stack>
#include <vector>
enum class Leaf {
    UNKNOWN = 0,
    SOLID = -1,
    EMPTY = -2,
};

struct BSPNode {
    BSPNode *front;
    BSPNode *back;
    Leaf leaf;
    size_t iteration;
};

class BSP {
    private:
        Leaf type_leaf(BSPNode bsp_node, Map *map);
        void create_bsp_tree();
        Map *map;
    public:
        std::vector<BSPNode> *last_node;
        std::vector<BSPNode> *bsp_node;
        std::stack<BSPNode> unresolved_nodes;
        BSP(Map *map);
    
};

