#include "game/map.hpp"
#include "game/player.hpp"
#include "input/input_service.hpp"
#include "platform/fixed_timestep.hpp"
#include "platform/sdl_runtime.hpp"
#include "rendering/graphics_window.hpp"

#include <SDL.h>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

using namespace std;

namespace croom {
namespace {
using Clock = chrono::steady_clock;
constexpr string_view base_path_failure = "finding the executable directory failed: ";

class GameLoop {
public:
  GameLoop(GraphicsWindow &window, Map &map, InputService &input)
      : window_(window), map_(map), input_(input) {}

  void run() {
    while (run_frame()) {
    }
  }

private:
  static constexpr FixedTimestep::Seconds fixed_time_step{1.0f / 60.0f};
  static constexpr size_t max_updates_per_frame = 8;

  bool run_frame() {
    const auto current_time = Clock::now();
    const FixedTimestep::Seconds elapsed = current_time - previous_time_;
    previous_time_ = current_time;
    const InputState input = input_.poll();
    if (input.quit_requested) {
      return false;
    }
    update_world(input, elapsed);
    window_.present(map_);
    const auto sleep_time =
        chrono::duration_cast<Clock::duration>(timestep_.time_until_next_update());
    this_thread::sleep_until(current_time + sleep_time);
    return true;
  }

  void update_world(const InputState &input, FixedTimestep::Seconds elapsed) {
    const PlayerCommand command{input.forward, input.strafe, input.turn};
    const size_t updates = timestep_.advance(elapsed);
    for (size_t update = 0; update < updates; ++update) {
      map_.update(command, fixed_time_step.count());
    }
  }

  GraphicsWindow &window_;
  Map &map_;
  InputService &input_;
  Clock::time_point previous_time_ = Clock::now();
  FixedTimestep timestep_{fixed_time_step, max_updates_per_frame};
};
}

class Application {
public:
  static int execute(int argc, char *argv[]) try {
    return Application{}.run(argc, argv);
  } catch (...) {
    return report_current_error();
  }

private:
  using BasePath = unique_ptr<char, decltype(&SDL_free)>;

  static int report_current_error() try {
    return print_error(current_error_message());
  } catch (...) {
    return print_error("CROOM failed with an unknown error");
  }

  static int print_error(string_view message) {
    cerr << message << '\n';
    return EXIT_FAILURE;
  }

  static string current_error_message() {
    try {
      throw;
    } catch (const exception &error) {
      return "CROOM failed: " + string{error.what()};
    }
  }

  int run(int argc, char *argv[]) {
    Map map;
    map.load_from_csv(requested_map_path(argc, argv));
    GameLoop{window_, map, input_}.run();
    return EXIT_SUCCESS;
  }

  BasePath executable_base_path() const {
    BasePath path{SDL_GetBasePath(), SDL_free};
    if (!path) {
      throw runtime_error(string{base_path_failure} + SDL_GetError());
    }
    return path;
  }

  filesystem::path default_map_path() const {
    return filesystem::path{executable_base_path().get()} / "assets" / "map.csv";
  }

  filesystem::path requested_map_path(int argc, char *argv[]) const {
    return argc > 1 ? filesystem::path{argv[1]} : default_map_path();
  }

  SdlRuntime sdl_;
  GraphicsWindow window_{"CROOM", 1000, 1000};
  InputService input_;
};

}

int main(int argc, char *argv[]) { return croom::Application::execute(argc, argv); }
