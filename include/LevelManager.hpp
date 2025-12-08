#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class LevelManager {
public:
    static LevelManager& getInstance() {
        static LevelManager instance;
        return instance;
    }

    void loadLevel(sf::RenderWindow& window, std::string levelPath);
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