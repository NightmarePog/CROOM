#pragma once

#include "input/input_service.hpp"
#include "platform/sdl_runtime.hpp"
#include "rendering/graphics_window.hpp"

#include <string>
#include <string_view>

namespace croom {

class Application {
public:
  static int execute(int argc, char *argv[]);

private:
  int run(int argc, char *argv[]);

  static int report_current_error();
  static int print_error(std::string_view message);
  static std::string current_error_message();

  SdlRuntime sdl_;
  GraphicsWindow window_{"CROOM", 1000, 1000};
  InputService input_;
};

}
