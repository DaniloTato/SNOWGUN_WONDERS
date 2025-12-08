#include <fstream>
#include "LevelManager.hpp"
#include "RenderableObject.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Constants.hpp"

void LevelManager::loadLevel(sf::RenderWindow& window, std::string levelPath) {
    std::ifstream levelFile(levelPath);
    if (!levelFile.is_open()) {
        throw std::runtime_error("Could not open level file: " + levelPath);
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(levelFile, line)) {
        levelLayout.push_back({});
        if (line.empty()) continue;
        for (int i = 0; i < line.size(); i++) {

            levelLayout.back().push_back(line[i] - '0');

            if (line[i] == '1') {

                sf::Texture tileTexture;
                tileTexture.loadFromFile("assets/tile.png");
                sf::IntRect tileRect(0, 0, Constants::TILE_SIZE, Constants::TILE_SIZE);

                RenderizerParameters tileParams{
                    window,
                    tileTexture,
                    tileRect,
                    {static_cast<float>(i * Constants::TILE_SIZE), static_cast<float>(lineNumber * Constants::TILE_SIZE)}
                };

                new RenderableObject(tileParams);
            }
        }
        lineNumber++;
    }

    levelFile.close();
}

const std::vector<std::vector<int>>& LevelManager::getLevelLayout() const{
    return levelLayout;
}