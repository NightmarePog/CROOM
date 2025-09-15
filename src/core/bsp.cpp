#include "core/bsp.hpp"
#include "game/sprite.hpp"
#include "game/map.hpp"
#include "utils/line_segment.hpp"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>


void print_node(BSPNode* node, int depth = 0) {
    if (!node) return;

    for (int i = 0; i < depth; ++i) std::cout << "  ";

    std::cout << "Node (Leaf: " << static_cast<int>(node->leaf)
              << ", Pos: (" << node->partition_segment.pos_a.x << "," << node->partition_segment.pos_a.y << "))\n";

    if (node->front) {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << "Front ->\n";
        print_node(node->front, depth + 1);
    }

    if (node->back) {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << "Back ->\n";
        print_node(node->back, depth + 1);
    }
}


BSP::BSP() {
    BSPNode* root_node = new BSPNode{
        nullptr,
        nullptr,
        Leaf::SOLID,
        LineSegment{{0,0},{0,0}},
        {}
    };
    node_queue.push(*root_node);
    std::cout << "BSP loaded :3" << std::endl;
}

void BSP::load_from_map(Map *map) {
   std::vector<Sprite*> sprites = map->get_entity_vector();
   for (Sprite* sprite : sprites) {
      this->currect_node.node_segments.push_back(sprite->get_cords());
   }
   this->is_loaded = true;
}

void BSP::create_bsp() {
    assert(this->is_loaded);
   print_node(&this->currect_node);
   std::cout << "no idea: " << this->currect_node.node_segments.front().pos_a.x << std::endl;
   
   currect_node = node_queue.front(); 
   node_queue.pop();
   currect_node.partition_segment = get_longest_segment(this->currect_node.node_segments);
   printf("test\n");
   Partition partition(currect_node.partition_segment);

   currect_node.front = new BSPNode;
   currect_node.back = new BSPNode;
   currect_node.front->leaf = Leaf::EMPTY;
   currect_node.back->leaf = Leaf::EMPTY;
   for (const LineSegment& segment : currect_node.node_segments) {
      if(partition.is_in_back(segment)) {
         currect_node.back->leaf = Leaf::SOLID;
         node_queue.push(*currect_node.back);
         printf("IN A BACK\n");
      }
      if(partition.is_in_front(segment)) {
         currect_node.front->leaf = Leaf::SOLID;
         node_queue.push(*currect_node.front);
         printf("IN A FRONT\n");
      }
   }


}


LineSegment BSP::get_longest_segment(std::vector<LineSegment> segments) {
    printf("[GET LONGEST] %f\n", segments.front().pos_a.x);
    if (segments.empty())
        throw std::runtime_error("No segments");

    LineSegment* longest = &segments[0];
    float max_length = 0.0f;

    // counting
    for (const LineSegment& seg : segments) {
        Vec2 delta = seg.pos_a - seg.pos_b;
        float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (length > max_length) {
            max_length = length;
            longest = const_cast<LineSegment*>(&seg);
        }
    }

    return *longest;
}


