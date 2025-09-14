#include "core/bsp.hpp"
#include "game/map.hpp"
#include "game/sprite.hpp"
#include "utils/vec2.hpp"
#include <vector>

BSP::BSP(Map *map) {
  this->map = map;
  std::vector<Sprite *> sprites = map->get_entity_vector();
  for (const auto &s : sprites) {
    Sprite_Cords cords = s->get_cords();
    this->candidates_pos.insert(cords.cord_a);
    this->candidates_pos.insert(cords.cord_b);
  }
  create_bsp_tree();
}

void BSP::create_bsp_tree() {
    BSPNode root_node = {
        nullptr,          // parent
        nullptr,          // children/nebo front/back
        Leaf::UNKNOWN,    // stav listu
        0                 // iterace
    };

    unresolved_nodes.push(root_node);

    while (!unresolved_nodes.empty()) {
        // vezmi aktuální uzel
        BSPNode curr_node = unresolved_nodes.top();
        unresolved_nodes.pop();

        // 1️⃣ najdi kandidáta a smaž ho ze setu
        Vec2 candidate = find_cut_candidate();
        delete_from_cand_set(candidate);

        // 2️⃣ otestuj, jestli je leaf
        if (type_leaf(curr_node, map)) {
            curr_node.leaf_state = Leaf::YES;
            continue; // uzel je leaf, nic dále neděláme
        }

        // 3️⃣ vytvoř potomky
        BSPNode front = curr_node; // nebo nastav podle rozdělení
        BSPNode back = curr_node;

        front.iteration = curr_node.iteration + 1;
        back.iteration = curr_node.iteration + 1;

        // 4️⃣ vlož potomky do stacku pro další zpracování
        unresolved_nodes.push(front);
        unresolved_nodes.push(back);
    }
}
Vec2 BSP::find_cut_candidate() {
  SplitAxis axis = choose_axis(this->iteration);
    int sumCoord = 0;

  auto get_coord = [axis](const Vec2 &c) -> int {
    return (axis == SplitAxis::X) ? c.x : c.y;
  };


  for (const auto &c : candidates_pos) {
    sumCoord += get_coord(c);
  }
  double center = sumCoord / static_cast<double>(candidates_pos.size());

  Vec2 bestCandidate = *candidates_pos.begin();
  double bestDist = std::abs(get_coord(bestCandidate) - center);

  for (const auto &c : candidates_pos) {
    double dist = std::abs(get_coord(c) - center);
    if (dist < bestDist) {
      bestDist = dist;
      bestCandidate = c;
    }
  }

  return bestCandidate;
}

SplitAxis BSP::choose_axis(int iteration) {
  if (iteration % 2 == 0) {
    return SplitAxis::X;
  } else {
    return SplitAxis::Y;
  }
}

void BSP::delete_from_cand_set(Vec2 vec2) {

    auto it = this->candidates_pos.find(vec2);

    if (it != this->candidates_pos.end()) {
        this->candidates_pos.erase(it);
    }
}

