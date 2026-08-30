cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER g++ CACHE STRING "C++ compiler")
endif()

project(CROOM VERSION 1.0 LANGUAGES CXX)

option(CROOM_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(CROOM_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED CONFIG)

if(TARGET SDL2::SDL2)
    set(CROOM_SDL_TARGET SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
    set(CROOM_SDL_TARGET SDL2::SDL2-static)
else()
    message(FATAL_ERROR "The SDL2 package does not provide SDL2::SDL2 or SDL2::SDL2-static")
endif()

add_library(croom_project_options INTERFACE)

set(CROOM_MSVC_FRONTEND OFF)
if(MSVC OR CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    set(CROOM_MSVC_FRONTEND ON)
endif()

if(CROOM_MSVC_FRONTEND)
    target_compile_options(croom_project_options INTERFACE
        /std:c++20
        /W4
        /permissive-
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(croom_project_options INTERFACE
        -std=c++20
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
    )
endif()

if(CROOM_WARNINGS_AS_ERRORS)
    if(CROOM_MSVC_FRONTEND)
        target_compile_options(croom_project_options INTERFACE /WX)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(croom_project_options INTERFACE -Werror)
    else()
        message(WARNING "CROOM_WARNINGS_AS_ERRORS is unsupported by this compiler")
    endif()
endif()

if(CROOM_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT CROOM_MSVC_FRONTEND)
        target_compile_options(croom_project_options INTERFACE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
        target_link_options(croom_project_options INTERFACE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
    else()
        message(WARNING
            "CROOM_ENABLE_SANITIZERS is supported only by GNU and Clang in non-MSVC mode"
        )
    endif()
endif()

function(croom_configure_target target)
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_link_libraries(${target} PRIVATE croom_project_options)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()

add_library(croom_game STATIC
    src/game/collision/collision_bsp.cpp
    src/game/collision/collision_bsp.hpp
    src/game/collision/collision_geometry.cpp
    src/game/collision/collision_geometry.hpp
    src/game/collision/collision_tree.cpp
    src/game/collision/collision_tree.hpp
    src/game/collision/movement_resolver.cpp
    src/game/collision/movement_resolver.hpp
    src/game/map.cpp
    src/game/map.hpp
    src/game/map_loader.cpp
    src/game/map_loader.hpp
    src/game/player.cpp
    src/game/player.hpp
    src/geometry/line_segment.hpp
    src/geometry/vec2.hpp
)
target_include_directories(croom_game PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(croom_game PUBLIC cxx_std_20)
croom_configure_target(croom_game)

add_library(croom_runtime STATIC
    src/input/input_service.cpp
    src/input/input_service.hpp
    src/platform/fixed_timestep.cpp
    src/platform/fixed_timestep.hpp
    src/platform/sdl_runtime.cpp
    src/platform/sdl_runtime.hpp
    src/rendering/graphics_window.cpp
    src/rendering/graphics_window.hpp
    src/rendering/renderer_2d.cpp
    src/rendering/renderer_2d.hpp
)
target_include_directories(croom_runtime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(croom_runtime PUBLIC cxx_std_20)
target_link_libraries(croom_runtime PUBLIC croom_game ${CROOM_SDL_TARGET})
croom_configure_target(croom_runtime)

add_executable(CROOM src/main.cpp)
set_target_properties(CROOM PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
foreach(configuration IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER "${configuration}" configuration_upper)
    set_target_properties(CROOM PROPERTIES
        "RUNTIME_OUTPUT_DIRECTORY_${configuration_upper}" "${CMAKE_CURRENT_BINARY_DIR}"
    )
endforeach()
if(TARGET SDL2::SDL2main)
    target_link_libraries(CROOM PRIVATE SDL2::SDL2main)
endif()
target_link_libraries(CROOM PRIVATE croom_runtime croom_game)
croom_configure_target(CROOM)

set(CROOM_MAP_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/map.csv")
set(CROOM_ASSET_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets")
file(MAKE_DIRECTORY "${CROOM_ASSET_DIRECTORY}")
configure_file("${CROOM_MAP_SOURCE}" "${CROOM_ASSET_DIRECTORY}/map.csv" COPYONLY)

add_custom_command(TARGET CROOM POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:CROOM>/assets"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${CROOM_MAP_SOURCE}"
        "$<TARGET_FILE_DIR:CROOM>/assets/map.csv"
    VERBATIM
)
