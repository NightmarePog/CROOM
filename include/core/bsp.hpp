#pragma  once

#include "game/map.hpp"
#include "utils/vec2.hpp"
#include <stack>
#include <vector>


enum class SplitAxis { X, Y };


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
    std::vector<Vec2> position_candidates;
};

class BSP {
    private:
        std::set<Vec2> candidates_pos;
        Leaf type_leaf(BSPNode bsp_node, Map *map);
        void create_bsp_tree();
        Vec2 find_cut_candidate();
        void delete_from_cand_set(Vec2 vec);
        int iteration;
        Map *map;
        SplitAxis choose_axis(int iteration);
    public:
        std::vector<BSPNode> *last_node;
        std::vector<BSPNode> *bsp_node;
        std::stack<BSPNode> unresolved_nodes;
        BSP(Map *map);
    
};

