#pragma once
#include <vector>
#include <string>
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "GameCamera.hpp"
#include "RenderableObject.hpp"

class LevelManager {
public:
    struct TileInfo {
        int x, y;
        sf::IntRect textureRect;
        RenderableObject* object = nullptr;
    };

    static LevelManager& getInstance();

    void loadLevel(sf::RenderWindow& window, GameCamera* camera, const std::string& path);

    void createTile(sf::RenderWindow& window, GameCamera* camera,
                    int x, int y, sf::IntRect rect);

    void deleteTile(int x, int y);
    void saveLevel(const std::string& path);

    const std::vector<std::vector<int>>& getLevelLayout() const;

    sf::Texture& getTilesheet();
    sf::IntRect selectedTileRect;

private:
    LevelManager();

    std::vector<TileInfo> tiles;
    sf::Texture tilesheet;

    std::vector<std::vector<int>> levelLayout;

    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;

    LevelManager(LevelManager&&) = delete;
    LevelManager& operator=(LevelManager&&) = delete;
};