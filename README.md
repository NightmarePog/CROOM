# CROOM

CROOM is a small SDL2 raycasting prototype for experimenting with first-person
rendering, movement, map loading, and BSP-accelerated collision detection.

## Requirements

- CMake 3.16 or newer
- GCC with C++20 support
- SDL2 development files
- Ninja (optional, but used in the examples)

On Ubuntu or Debian, install the toolchain with:

```sh
sudo apt-get install build-essential cmake libsdl2-dev ninja-build
```

## Build and run

Configure and build a debug executable:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/CROOM
```

The default map is copied beside the executable, so `CROOM` can be launched from
any working directory. Pass a CSV path to load a different map:

```sh
./build/CROOM /path/to/map.csv
```

Map files contain one wall per line as `start_x,start_y,end_x,end_y`.

For an optimized build:

```sh
cmake -S . -B build-release -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++
cmake --build build-release
./build-release/CROOM
```

Omit `-G Ninja` if you prefer CMake's default generator.

## Sanitizers

AddressSanitizer and UndefinedBehaviorSanitizer are available with GCC:

```sh
cmake -S . -B build-sanitize -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=g++ \
  -DCROOM_ENABLE_SANITIZERS=ON
cmake --build build-sanitize
```

## Controls

- `W` / `S`: move forward / backward
- `A` / `D`: strafe left / right
- Left / Right Arrow: turn
- `Q` or close the window: quit

## Architecture

- `src/main.cpp` owns the application loop and is the only source directly under
  `src/`.
- `src/game/` contains the map, player, and collision code. `CollisionBsp` is a
  small facade over geometry checks, the collision tree, and movement resolution.
- `src/geometry/` contains the small value types shared by gameplay and rendering.
- `src/input/`, `src/platform/`, and `src/rendering/` contain SDL-facing runtime
  code. `Raycaster` handles ray/segment intersections, `RaycastProjection`
  converts those hits into screen columns, and `Renderer3D` only draws the
  resulting first-person view. Maps do not need to use a tile grid.

Files are grouped by domain, with matching headers and implementations beside
each other, such as `src/game/map.hpp` and `src/game/map.cpp`. Related collision
components share `src/game/collision/`.
