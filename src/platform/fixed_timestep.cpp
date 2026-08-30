#include "platform/fixed_timestep.hpp"

#include <cmath>
#include <stdexcept>

using namespace std;

namespace croom {
FixedTimestep::FixedTimestep(Seconds step, size_t maximum_updates)
    : step_(step), maximum_updates_(maximum_updates) {
  const float maximum_seconds = step.count() * static_cast<float>(maximum_updates);
  if (!isfinite(step.count()) || step <= Seconds::zero() || maximum_updates == 0 ||
      !isfinite(maximum_seconds)) {
    throw invalid_argument("fixed timestep requires a finite step and update limit");
  }
}

size_t FixedTimestep::advance(Seconds elapsed) {
  if (!isfinite(elapsed.count()) || elapsed < Seconds::zero()) {
    throw invalid_argument(
        "fixed timestep elapsed time must be finite and nonnegative");
  }
  const Seconds limit = step_ * static_cast<float>(maximum_updates_);
  if (elapsed >= limit - accumulator_) {
    accumulator_ = Seconds::zero();
    return maximum_updates_;
  }
  accumulator_ += elapsed;
  size_t updates = 0;
  while (accumulator_ >= step_) {
    accumulator_ -= step_;
    ++updates;
  }
  return updates;
}

}
