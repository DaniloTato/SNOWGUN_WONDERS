#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "GameCamera.hpp"

class LevelManager {
public:
    static LevelManager& getInstance();
    void loadLevel(sf::RenderWindow& window, GameCamera* camera, std::string levelPath);
    const std::vector<std::vector<int>>& getLevelLayout() const;

private:

    std::vector<std::vector<int>> levelLayout;

    LevelManager() = default;
    ~LevelManager() = default;

    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;

    LevelManager(LevelManager&&) = delete;
    LevelManager& operator=(LevelManager&&) = delete;
};