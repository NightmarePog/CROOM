#include "game/map.hpp"

#include "game/map_loader.hpp"

using namespace std;

namespace croom {

void Map::load_from_csv(const filesystem::path &path) {
  collisions_ = MapLoader::load(path, player_.position());
}

}
