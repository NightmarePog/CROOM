#include "game/game_loop.hpp"

#include "game/map.hpp"
#include "game/player.hpp"
#include "input/input_service.hpp"
#include "rendering/graphics_window.hpp"

#include <thread>

using namespace std;

namespace croom {

GameLoop::GameLoop(GraphicsWindow &window, Map &map, InputService &input)
    : window_{window}, map_{map}, input_{input} {}

void GameLoop::run() {
  while (run_frame()) {
  }
}

FixedTimestep::Seconds GameLoop::begin_frame(Clock::time_point now) {
  const FixedTimestep::Seconds elapsed = now - previous_time_;
  previous_time_ = now;
  return elapsed;
}

void GameLoop::update_world(const InputState &input,
                            FixedTimestep::Seconds elapsed) {
  const PlayerCommand command{input.forward, input.strafe, input.turn};
  for (size_t update = 0; update < timestep_.advance(elapsed); ++update)
    map_.update(command, fixed_time_step.count());
}

void GameLoop::wait_for_next_update(Clock::time_point frame_started) const {
  this_thread::sleep_until(
      frame_started +
      chrono::duration_cast<Clock::duration>(timestep_.time_until_next_update()));
}

bool GameLoop::run_frame() {
  const Clock::time_point frame_started = Clock::now();
  const FixedTimestep::Seconds elapsed = begin_frame(frame_started);
  const InputState input = input_.poll();
  if (input.quit_requested)
    return false;

  update_world(input, elapsed);
  window_.present(map_);
  wait_for_next_update(frame_started);
  return true;
}

}
