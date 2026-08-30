#pragma once

#include <atomic>

namespace croom {

class SdlRuntime {
public:
  SdlRuntime();
  ~SdlRuntime();

  SdlRuntime(const SdlRuntime &) = delete;
  SdlRuntime &operator=(const SdlRuntime &) = delete;
  SdlRuntime(SdlRuntime &&) = delete;
  SdlRuntime &operator=(SdlRuntime &&) = delete;

private:
  static void claim();
  [[noreturn]] static void fail_initialization();

  static std::atomic_bool active_;
};

}
