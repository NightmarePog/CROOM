#pragma  once

#include "game/map.hpp"
#include "utils/line_segment.hpp"
#include <memory>
#include <queue>
#include <vector>


/** BSP tree node data structure */
struct BSPNode {
    std::unique_ptr<BSPNode> front;
    std::unique_ptr<BSPNode> back;
    Partition* partition;
    std::vector<LineSegment> segments;

};


/** algorithm that build a BSP tree
*/
class BSP {
    private: 
        /**
        says if any map is loaded
        */
        bool is_loaded;
        /**
        root node of BSP tree
        */
        BSPNode* root;
        /**
        gets a longest segment in vector
        @param std::vector<LineSegment> vector of segments
        @returns longest LineSegment
        */
        LineSegment get_longest_segment(std::vector<LineSegment> segments);
        /**
        processes a single BSP node
        */
        void process_node(BSPNode *node);
        /**
        queue of unfinished BSP nodes 
        */
        std::queue <BSPNode*> queue;
    public:
        BSP();
        /** loads the map from CSV file */
        void load_from_csv();
        /** TODO, might be removed */
        void load_from_map(Map *map);
        /** according to imported map it tries to build a bsp tree */
        void build_bsp();
};

