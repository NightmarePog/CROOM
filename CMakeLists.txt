cmake_minimum_required(VERSION 3.16)
project(CROOM VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED CONFIG)

add_executable(CROOM
    src/main.cpp
    src/data_structures/binary_tree/binary_tree.cpp
    src/game/bsp/bsp.cpp
    src/game/map/map.cpp
    src/game/player/player.cpp
    src/game/raycast/raycast.cpp
    src/game/sprite/sprite.cpp
    src/geometry/partition/partition.cpp
    src/input/user_input/user_input.cpp
    src/rendering/graphics_window/graphics_window.cpp
    src/rendering/renderer_2d/renderer_2d.cpp
)

target_compile_features(CROOM PRIVATE cxx_std_20)
target_include_directories(CROOM PRIVATE src)
target_link_libraries(CROOM PRIVATE SDL2::SDL2)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(CROOM PRIVATE -Wall -Wextra -Wpedantic)
endif()
