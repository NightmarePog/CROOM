#include "game/map.hpp"
#include "game/sprite.hpp"
#include "utils/vec2.hpp"
#include <vector>

Map::Map(Vec2 map_size) {
    this->map_size = map_size;
}

void Map::remove_entity(int id) {
    if (id >= 0 && id < sprite_vec.size()) {
        sprite_vec.erase(sprite_vec.begin()+ id);
    }
}

int Map::add_entity(Sprite *sprite) {
    sprite_vec.push_back(std::unique_ptr<Sprite>(sprite));
    return sprite_vec.size()-1;
}

void Map::clear_entities() {
    sprite_vec.clear();
}

const std::vector<std::unique_ptr<Sprite>>& Map::get_entity_vector() {
    return sprite_vec;
}