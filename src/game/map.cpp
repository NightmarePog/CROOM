#include "game/map.hpp"

#include "game/map_loader.hpp"

using namespace std;

namespace croom {

Map::Map(const filesystem::path &path)
    : collisions_{MapLoader::load(path, player_.position())} {}

}
