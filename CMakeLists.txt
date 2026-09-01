cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_COMPILER g++ CACHE STRING "C++ compiler")
project(CROOM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CROOM_ENABLE_SANITIZERS "Enable runtime sanitizers" OFF)

find_package(SDL2 REQUIRED CONFIG)

add_executable(CROOM
    src/main.cpp
    src/app/application.cpp
    src/app/map_path.cpp
    src/game/collision/circle_sweep.cpp
    src/game/collision/collision_bsp.cpp
    src/game/collision/collision_geometry.cpp
    src/game/collision/collision_tree.cpp
    src/game/collision/collision_tree_builder.cpp
    src/game/collision/contact_manifold.cpp
    src/game/collision/movement_resolver.cpp
    src/game/map.cpp
    src/game/map_loader.cpp
    src/game/map_row_parser.cpp
    src/game/game_loop.cpp
    src/game/player.cpp
    src/input/input_service.cpp
    src/platform/fixed_timestep.cpp
    src/platform/sdl_runtime.cpp
    src/rendering/graphics_window.cpp
    src/rendering/raycast_projection.cpp
    src/rendering/raycaster.cpp
    src/rendering/renderer_3d.cpp
    src/rendering/sdl_renderer_factory.cpp
)

target_include_directories(CROOM PRIVATE src)
target_link_libraries(CROOM PRIVATE SDL2::SDL2)
target_compile_options(CROOM PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Werror
)

if(CROOM_ENABLE_SANITIZERS)
    target_compile_options(CROOM PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
    target_link_options(CROOM PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets")
configure_file(assets/map.csv assets/map.csv COPYONLY)
