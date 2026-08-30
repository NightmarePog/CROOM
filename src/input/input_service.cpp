#include "input/input_service.hpp"

#include <SDL.h>
#include <algorithm>
#include <cmath>

using namespace std;

namespace croom {
InputService::InputService() : keyboard_(SDL_GetKeyboardState(nullptr)) {}

float InputService::read_axis(int positive_key, int negative_key) const {
  return static_cast<float>(key_down(positive_key)) -
         static_cast<float>(key_down(negative_key));
}

bool InputService::key_down(int key) const { return keyboard_[key] != 0; }

bool InputService::requests_quit(const SDL_Event &event) {
  return event.type == SDL_QUIT ||
         (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_Q);
}

void InputService::normalize_movement(InputState &input) {
  const float scale = max(hypot(input.forward, input.strafe), 1.0f);
  input.forward /= scale;
  input.strafe /= scale;
}

InputState InputService::poll() {
  bool quit_requested = false;
  SDL_Event event{};
  while (SDL_PollEvent(&event) != 0) {
    quit_requested = quit_requested || requests_quit(event);
  }
  InputState input{read_axis(SDL_SCANCODE_W, SDL_SCANCODE_S),
                   read_axis(SDL_SCANCODE_D, SDL_SCANCODE_A),
                   read_axis(SDL_SCANCODE_RIGHT, SDL_SCANCODE_LEFT),
                   quit_requested || key_down(SDL_SCANCODE_Q)};
  normalize_movement(input);
  return input;
}

}
