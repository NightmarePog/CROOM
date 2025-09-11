cmake_minimum_required(VERSION 3.16)
project(Raycast++ CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include_directories(include)
file(GLOB_RECURSE SOURCES src/core/*.cpp)

add_executable(Raycast_plusplus ${SOURCES})

find_package(SDL2 REQUIRED)

if (SDL2_FOUND)
    target_include_directories(Raycast_plusplus PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(Raycast_plusplus ${SDL2_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()
