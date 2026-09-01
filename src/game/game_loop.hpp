#pragma once

#include "platform/fixed_timestep.hpp"

#include <chrono>
#include <cstddef>

namespace croom {

class GraphicsWindow;
class InputService;
struct InputState;
class Map;

class GameLoop {
public:
  GameLoop(GraphicsWindow &window, Map &map, InputService &input);

  void run();

private:
  using Clock = std::chrono::steady_clock;

  static constexpr FixedTimestep::Seconds fixed_time_step{1.0f / 60.0f};
  static constexpr std::size_t maximum_updates_per_frame = 8;

  bool run_frame();
  FixedTimestep::Seconds begin_frame(Clock::time_point now);
  void update_world(const InputState &input, FixedTimestep::Seconds elapsed);
  void wait_for_next_update(Clock::time_point frame_started) const;

  GraphicsWindow &window_;
  Map &map_;
  InputService &input_;
  Clock::time_point previous_time_ = Clock::now();
  FixedTimestep timestep_{fixed_time_step, maximum_updates_per_frame};
};

}
