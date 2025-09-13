cmake_minimum_required(VERSION 3.16)
project(RaycastPlusPlus VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE CORE_SOURCES ${CMAKE_SOURCE_DIR}/src/core/*.cpp)
file(GLOB_RECURSE GAME_SOURCES ${CMAKE_SOURCE_DIR}/src/game/*.cpp)
file(GLOB_RECURSE UTILS_SOURCES ${CMAKE_SOURCE_DIR}/src/utils/*.cpp)

add_executable(Raycast_plusplus
    ${CORE_SOURCES}
    ${GAME_SOURCES}
    ${UTILS_SOURCES}
)

# SDL2
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    target_include_directories(Raycast_plusplus PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(Raycast_plusplus PRIVATE ${SDL2_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()

# SDL2_image 
target_include_directories(Raycast_plusplus PRIVATE /usr/include/SDL2)
target_link_libraries(Raycast_plusplus PRIVATE SDL2_image)
