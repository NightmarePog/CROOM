#include "app/application.hpp"

#include "app/map_path.hpp"
#include "game/game_loop.hpp"
#include "game/map.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

namespace croom {

int Application::execute(int argc, char *argv[]) try {
  return Application{}.run(argc, argv);
} catch (...) {
  return report_current_error();
}

int Application::run(int argc, char *argv[]) {
  Map map{requested_map_path(argc, argv)};
  GameLoop{window_, map, input_}.run();
  return EXIT_SUCCESS;
}

int Application::report_current_error() try {
  return print_error(current_error_message());
} catch (...) {
  return print_error("CROOM failed with an unknown error");
}

int Application::print_error(string_view message) {
  cerr << message << '\n';
  return EXIT_FAILURE;
}

string Application::current_error_message() {
  try {
    throw;
  } catch (const exception &error) {
    return "CROOM failed: " + string{error.what()};
  }
}

}
