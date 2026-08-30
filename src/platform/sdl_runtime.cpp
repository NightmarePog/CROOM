#include "platform/sdl_runtime.hpp"

#include <SDL.h>
#include <atomic>
#include <stdexcept>
#include <string>

using namespace std;

namespace croom {
atomic_bool SdlRuntime::active_ = false;

void SdlRuntime::claim() {
  bool expected = false;
  if (!active_.compare_exchange_strong(expected, true))
    throw logic_error("only one SDL runtime may be active");
}

void SdlRuntime::fail_initialization() {
  const string error = SDL_GetError();
  SDL_Quit();
  active_ = false;
  throw runtime_error("SDL video initialization failed: " + error);
}

SdlRuntime::SdlRuntime() {
  claim();
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    fail_initialization();
}

SdlRuntime::~SdlRuntime() {
  SDL_Quit();
  active_ = false;
}

}
