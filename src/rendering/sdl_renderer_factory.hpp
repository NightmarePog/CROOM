#pragma once

#include <cstdint>
#include <string>

struct SDL_Renderer;
struct SDL_Window;

namespace croom {

class SdlRendererFactory {
public:
  explicit SdlRendererFactory(SDL_Window &window);

  SDL_Renderer *create();

private:
  SDL_Renderer *try_create(std::uint32_t flags) const;
  SDL_Renderer *create_accelerated();
  SDL_Renderer *create_software();
  SDL_Renderer *create_fallback();
  std::string failure_message() const;

  SDL_Window *window_;
  std::string accelerated_error_;
  std::string software_error_;
};

}
