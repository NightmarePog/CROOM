# CROOM

## Description

CROOM is an experimental 3D rendering engine written in C++ and SDL2. It uses
column-based rendering, similar to the technique used by 1993's *Doom*.

The project is under development.

## Building

Requirements: a C++20 compiler, CMake 3.16 or newer, and SDL2.

```sh
cmake -S . -B build -G Ninja
cmake --build build
cd build
./CROOM
```

Ninja is optional; omit `-G Ninja` to use CMake's default generator.

## Project structure

`src/main.cpp` owns the application state and is the only file directly under
`src/`. Other code is grouped by domain, then by component. Headers live beside
their implementations in same-named component directories, for example
`src/game/map/map.{hpp,cpp}` and
`src/rendering/renderer_2d/renderer_2d.{hpp,cpp}`. Header-only components follow
the same layout, such as `src/geometry/vec2/vec2.hpp`.
