#pragma once

#include "SFML/System/Vector2.hpp"
#include <string>

namespace Constants {
    inline constexpr int TILE_SIZE = 16;
    inline constexpr int COLLISION_GRID_WIDTH = 4;
    inline constexpr int COLLISION_GRID_HEIGHT = 4;
    inline constexpr int SCREEN_WIDTH = 800;
    inline constexpr int SCREEN_HEIGHT = 600;
    inline constexpr int FRAME_RATE = 60;
    const std::string TILESET_PATH = "./assets/house_spritesheet.png";
    const std::string STARTING_LEVEL_PATH = "./assets/level_data/barracks.json";
    const sf::Vector2f STARTING_PLAYER_CAMERA_RELATION = {20,-20};
}