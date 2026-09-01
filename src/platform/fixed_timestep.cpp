#include "platform/fixed_timestep.hpp"

#include <cmath>
#include <stdexcept>

using namespace std;

namespace croom {

void FixedTimestep::validate_configuration(Seconds step, size_t maximum_updates) {
  const float maximum_seconds = step.count() * static_cast<float>(maximum_updates);
  if (!isfinite(step.count()) || step <= Seconds::zero() || maximum_updates == 0 ||
      !isfinite(maximum_seconds))
    throw invalid_argument("fixed timestep requires a finite step and update limit");
}

void FixedTimestep::validate_elapsed(Seconds elapsed) {
  if (!isfinite(elapsed.count()) || elapsed < Seconds::zero())
    throw invalid_argument(
        "fixed timestep elapsed time must be finite and nonnegative");
}

FixedTimestep::FixedTimestep(Seconds step, size_t maximum_updates)
    : step_(step), maximum_updates_(maximum_updates) {
  validate_configuration(step, maximum_updates);
}

FixedTimestep::Seconds FixedTimestep::update_limit() const {
  return step_ * static_cast<float>(maximum_updates_);
}

bool FixedTimestep::reaches_update_limit(Seconds elapsed) const {
  return elapsed >= update_limit() - accumulator_;
}

size_t FixedTimestep::consume_updates() {
  size_t updates = 0;
  while (accumulator_ >= step_) {
    accumulator_ -= step_;
    ++updates;
  }
  return updates;
}

size_t FixedTimestep::advance(Seconds elapsed) {
  validate_elapsed(elapsed);
  if (reaches_update_limit(elapsed)) {
    accumulator_ = Seconds::zero();
    return maximum_updates_;
  }

  accumulator_ += elapsed;
  return consume_updates();
}

}
