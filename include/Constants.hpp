#pragma once

#include <string>

namespace Constants {
    inline constexpr int TILE_SIZE = 16;
    inline constexpr int COLLISION_GRID_WIDTH = 4;
    inline constexpr int COLLISION_GRID_HEIGHT = 4;
    inline constexpr int SCREEN_WIDTH = 800;
    inline constexpr int SCREEN_HEIGHT = 600;
    inline constexpr int FRAME_RATE = 60;
    const std::string TILESET_PATH = "./assets/tileset.png";
}