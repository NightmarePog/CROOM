#include "rendering/sdl_renderer_factory.hpp"

#include <SDL.h>
#include <stdexcept>

using namespace std;

namespace croom {

SdlRendererFactory::SdlRendererFactory(SDL_Window &window) : window_{&window} {}

SDL_Renderer *SdlRendererFactory::try_create(uint32_t flags) const {
  return SDL_CreateRenderer(window_, -1, flags);
}

SDL_Renderer *SdlRendererFactory::create_accelerated() {
  if (SDL_Renderer *renderer = try_create(SDL_RENDERER_ACCELERATED))
    return renderer;
  accelerated_error_ = SDL_GetError();
  return create_software();
}

SDL_Renderer *SdlRendererFactory::create_software() {
  if (SDL_Renderer *renderer = try_create(SDL_RENDERER_SOFTWARE))
    return renderer;
  software_error_ = SDL_GetError();
  return create_fallback();
}

SDL_Renderer *SdlRendererFactory::create_fallback() {
  if (SDL_Renderer *renderer = try_create(0))
    return renderer;
  throw runtime_error(failure_message());
}

string SdlRendererFactory::failure_message() const {
  return "SDL renderer creation failed (accelerated: " + accelerated_error_ +
         "; software: " + software_error_ + "; default: " + SDL_GetError() + ")";
}

SDL_Renderer *SdlRendererFactory::create() { return create_accelerated(); }

}
