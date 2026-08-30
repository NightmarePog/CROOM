#pragma once

#include <cstdint>

union SDL_Event;

namespace croom {

struct InputState {
  float forward = 0.0f;
  float strafe = 0.0f;
  float turn = 0.0f;
  bool quit_requested = false;
};

class InputService {
public:
  InputService();

  InputState poll();

private:
  float read_axis(int positive_key, int negative_key) const;
  bool key_down(int key) const;
  static bool requests_quit(const SDL_Event &event);
  static void normalize_movement(InputState &input);

  const std::uint8_t *keyboard_;
};

}
