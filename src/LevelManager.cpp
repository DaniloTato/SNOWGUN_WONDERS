#include <fstream>
#include "LevelManager.hpp"
#include "RenderableObject.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Constants.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

LevelManager::LevelManager() {}

LevelManager& LevelManager::getInstance() {
    static LevelManager instance;
    return instance;
}

void LevelManager::loadLevel(sf::RenderWindow& window, GameCamera* camera, const std::string& path)
{
    tiles.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file: " + path);
    }

    json data = json::parse(file);

    tilesheet.loadFromFile(data["tileset"].get<std::string>());

    int tileSize = data["tile_size"];

    for (auto& t : data["tiles"])
    {
        TileInfo info;
        info.x     = t["x"];
        info.y     = t["y"];

        info.textureRect = sf::IntRect(t["tex_x"], t["tex_y"], tileSize, tileSize);

        RenderizerParameters params{
            window,
            tilesheet,
            info.textureRect,
            {
                float(info.x * tileSize),
                float(info.y * tileSize)
            }
        };

        info.object = new RenderableObject(params);
        info.object->renderizer.assignCamera(camera);

        tiles.push_back(info);
    }

    int width  = 200;
    int height = 200;

    levelLayout.assign(height, std::vector<int>(width, 0));

    for (const auto& t : tiles) {
        levelLayout[t.y][t.x] = 1;
    }
}

void LevelManager::createTile(sf::RenderWindow& window, GameCamera* camera,
                              int x, int y, sf::IntRect rect)
{
    if (x < 0 || y < 0) return;

    for (auto& t : tiles)
    {
        if (t.x == x && t.y == y)
        {
            levelLayout[y][x] = 1;

            if (t.object) {
                GameObject::destroy(t.object);
            }

            t.textureRect = sf::IntRect(rect.left, rect.top, Constants::TILE_SIZE, Constants::TILE_SIZE);

            RenderizerParameters params{
                window,
                tilesheet,
                t.textureRect,
                {
                    float(x * Constants::TILE_SIZE),
                    float(y * Constants::TILE_SIZE)
                }
            };

            t.object = new RenderableObject(params);
            t.object->renderizer.assignCamera(camera);

            return;
        }
    }

    TileInfo info;
    info.x = x;
    info.y = y;

    info.textureRect = sf::IntRect(rect.left, rect.top, Constants::TILE_SIZE, Constants::TILE_SIZE);

    levelLayout[y][x] = 1;

    RenderizerParameters params{
        window,
        tilesheet,
        info.textureRect,
        {
            float(x * Constants::TILE_SIZE),
            float(y * Constants::TILE_SIZE)
        }
    };

    info.object = new RenderableObject(params);
    info.object->renderizer.assignCamera(camera);

    tiles.push_back(info);
}

void LevelManager::deleteTile(int x, int y)
{
    if (x < 0 || y < 0) return;

    tiles.erase(
        std::remove_if(tiles.begin(), tiles.end(),
            [&](TileInfo& t){
                if (t.x == x && t.y == y) {
                    if (t.object) {
                        GameObject::destroy(t.object);
                        t.object = nullptr;
                    }
                    return true;
                }
                return false;
            }
        ),
        tiles.end()
    );

    if (y < levelLayout.size() && x < levelLayout[y].size()){
        levelLayout[y][x] = 0;
    }
}

void LevelManager::saveLevel(const std::string& path)
{
    json data;
    data["tile_size"] = Constants::TILE_SIZE;
    data["tileset"]   = Constants::TILESET_PATH;

    for (const auto& info : tiles) {
        data["tiles"].push_back({
            {"x",     info.x},
            {"y",     info.y},
            {"tex_x", info.textureRect.left},
            {"tex_y", info.textureRect.top}
        });
    }

    std::ofstream file(path);
    file << data.dump(4);

    std::cout << "Level saved to " << path << std::endl;
}

const std::vector<std::vector<int>>& LevelManager::getLevelLayout() const {
    return levelLayout;
}

sf::Texture& LevelManager::getTilesheet() {
    return tilesheet;
}