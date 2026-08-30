#pragma once

#include <chrono>
#include <cstddef>

namespace croom {

class FixedTimestep {
public:
  using Seconds = std::chrono::duration<float>;

  FixedTimestep(Seconds step, std::size_t maximum_updates);

  std::size_t advance(Seconds elapsed);
  constexpr Seconds time_until_next_update() const { return step_ - accumulator_; }

private:
  Seconds step_;
  Seconds accumulator_ = Seconds::zero();
  std::size_t maximum_updates_;
};

}
