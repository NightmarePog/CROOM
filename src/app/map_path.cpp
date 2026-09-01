#include "app/map_path.hpp"

#include <SDL.h>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

namespace croom {
namespace {
using BasePath = unique_ptr<char, decltype(&SDL_free)>;

filesystem::path executable_directory() {
  BasePath path{SDL_GetBasePath(), SDL_free};
  if (!path)
    throw runtime_error("finding the executable directory failed: " +
                        string{SDL_GetError()});
  return path.get();
}

filesystem::path default_map_path() {
  return executable_directory() / "assets" / "map.csv";
}
}

filesystem::path requested_map_path(int argc, char *argv[]) {
  return argc > 1 ? filesystem::path{argv[1]} : default_map_path();
}

}
