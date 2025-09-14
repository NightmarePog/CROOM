#include "core/bsp.hpp"
#include "game/map.hpp"
#include "utils/vec2.hpp"

BSP::BSP(Map *map) {
    this->map = map;
    create_bsp_tree();
}

void BSP::create_bsp_tree() {
    BSPNode root_node = {
        nullptr,
        nullptr,
        Leaf::UNKNOWN,
        0,
    };
    unresolved_nodes.push(root_node);

    while (!unresolved_nodes.empty()) {
    }
}


Leaf BSP::type_leaf(BSPNode bsp_node, Map *map) {
        Vec2 map_size = map->get_map_size();
        BSPNode curr_node = unresolved_nodes.top();

        // Určení rozsahu smyček podle parity iterace
        int max_x = (curr_node.iteration % 2 == 1) ? (map_size.x / curr_node.iteration) : map_size.x;
        int max_y = (curr_node.iteration % 2 == 1) ? map_size.y : (map_size.y / curr_node.iteration);

        // Procházení všech bodů v aktuálním výřezu
        for (int x = 0; x < max_x; x++) {
            for (int y = 0; y < max_y; y++) {
                if (map->needs_division(x, y)) {
                    return false; // bod vyžaduje další dělení → není leaf
                }
            }
        }

        return true; // všechny body jsou „leaf“ → uzel je leaf
    }
};


