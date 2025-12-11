#include <fstream>
#include "LevelManager.hpp"
#include "RenderableObject.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Constants.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

LevelManager::LevelManager(): selectedTileRect(sf::IntRect(16,16,16,16)), activeLayer(0){}

LevelManager& LevelManager::getInstance() {
    static LevelManager instance;
    return instance;
}

void LevelManager::loadLevel(sf::RenderWindow& window, GameCamera* camera, const std::string& path){
    layers.clear();

    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open level file: " + path);

    json data = json::parse(file);

    tilesheet.loadFromFile(data["tileset"].get<std::string>());
    int tileSize = data["tile_size"];

    auto& jsonLayers = data["layers"];
    layers.resize(jsonLayers.size());

    int width  = 200;
    int height = 200;
    levelLayout.assign(height, std::vector<int>(width, 0));

    for (int i = 0; i < jsonLayers.size(); i++) {
        layers[i].name = jsonLayers[i]["name"].get<std::string>();
        layers[i].paralax = jsonLayers[i]["parallax"].get<float>();
        loadLayer(window, camera, i, jsonLayers[i], tileSize);
    }

    std::cout << "totalLayers -> " << layers.size() << "\n";
}

void LevelManager::deleteLayerObjects(int layerNo) {
    LayerInfo layer = layers[layerNo];

    for (auto& tile : layer.tiles) {
        if (tile.object) {
            GameObject::destroy(tile.object);
            tile.object = nullptr;
        }
    }
}

void LevelManager::loadLayer(sf::RenderWindow& window,
                             GameCamera* camera,
                             int layerNo,
                             const json& layerJSON,
                             int tileSize)
{
    auto& tileList = layers[layerNo].tiles;
    tileList.clear();

    const auto& jsonTiles = layerJSON["tiles"];

    for (const auto& t : jsonTiles)
    {
        TileInfo info;
        info.x = t["x"].get<int>();
        info.y = t["y"].get<int>();

        info.textureRect = sf::IntRect(
            t["tex_x"].get<int>(),
            t["tex_y"].get<int>(),
            tileSize,
            tileSize
        );

        RenderizerParameters params{
            window,
            tilesheet,
            info.textureRect,
            {
                float(info.x * tileSize),
                float(info.y * tileSize)
            },
            camera,
            static_cast<float>(layerNo),
            layers[layerNo].paralax
        };

        info.object = new RenderableObject(params);

        tileList.push_back(info);

        if (layerNo == 0 && info.y < levelLayout.size() && info.x < levelLayout[0].size())
            levelLayout[info.y][info.x] = 1;
    }
}

void LevelManager::reloadAllLayers(sf::RenderWindow& window, GameCamera* camera){
    for (int i = 0; i < layers.size(); ++i)
        reloadLayer(window, camera, i);
}

void LevelManager::reloadLayer(sf::RenderWindow& window, GameCamera* camera, int layerNo){
    auto& tiles = layers[layerNo].tiles;

    for (auto& t : tiles)
    {
        if (t.object) {
            GameObject::destroy(t.object);
            t.object = nullptr;
        }

        RenderizerParameters params{
            window,
            tilesheet,
            t.textureRect,
            {
                float(t.x * Constants::TILE_SIZE),
                float(t.y * Constants::TILE_SIZE)
            },
            camera,
            static_cast<float>(layerNo),
            layers[layerNo].paralax
        };

        t.object = new RenderableObject(params);
    }

    std::cout << "Reloaded layer #" << layerNo << ", tiles: "
              << tiles.size() << "\n";
}

void LevelManager::createTile(sf::RenderWindow& window, GameCamera* camera, int layerNo,
                              int x, int y, sf::IntRect rect)
{
    if (x < 0 || y < 0) return;

    std::vector<TileInfo>& tiles = layers[layerNo].tiles;

    for (auto& t : tiles)
    {
        if (t.x == x && t.y == y)
        {
            if(layerNo == 0){
                levelLayout[y][x] = 1;
            }

            if (t.object) {
                GameObject::destroy(t.object);
            }

            t.textureRect = rect;

            RenderizerParameters params{
                window,
                tilesheet,
                t.textureRect,
                {
                    float(x * Constants::TILE_SIZE),
                    float(y * Constants::TILE_SIZE)
                },
                camera,
                static_cast<float>(layerNo),
                layers[layerNo].paralax
            };

            t.object = new RenderableObject(params);

            return;
        }
    }

    TileInfo info;
    info.x = x;
    info.y = y;

    info.textureRect = sf::IntRect(rect.left, rect.top, Constants::TILE_SIZE, Constants::TILE_SIZE);

    if(layerNo == 0){
        levelLayout[y][x] = 1;
    }

    RenderizerParameters params{
        window,
        tilesheet,
        info.textureRect,
        {
            float(x * Constants::TILE_SIZE),
            float(y * Constants::TILE_SIZE)
        },
        camera,
        static_cast<float>(layerNo),
        layers[layerNo].paralax
    };

    info.object = new RenderableObject(params);
    info.object->renderizer.assignCamera(camera);
    tiles.push_back(info);
}

void LevelManager::deleteTile(int layerNo, int x, int y){
    if (x < 0 || y < 0) return;

    std::vector<TileInfo>& tiles = layers[layerNo].tiles;

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

    if (layerNo == 0 && y < levelLayout.size() && x < levelLayout[y].size()){
        levelLayout[y][x] = 0;
    }
}

void LevelManager::saveLevel(const std::string& path)
{
    json data;

    data["tile_size"] = Constants::TILE_SIZE;
    data["tileset"]   = Constants::TILESET_PATH;

    data["layers"] = json::array();

    for (const auto& layer : layers)
    {
        json layerJson;
        layerJson["name"]     = layer.name;
        layerJson["parallax"] = layer.paralax;

        // Now save this layer's tiles
        layerJson["tiles"] = json::array();
        for (const auto& tile : layer.tiles)
        {
            layerJson["tiles"].push_back({
                {"x",     tile.x},
                {"y",     tile.y},
                {"tex_x", tile.textureRect.left},
                {"tex_y", tile.textureRect.top}
            });
        }

        data["layers"].push_back(layerJson);
    }

    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << path << " for writing!" << std::endl;
        return;
    }

    file << data.dump(4);
    file.close();
    std::cout << "Level saved to " << path << std::endl;
}

const std::vector<std::vector<int>>& LevelManager::getLevelLayout() const {
    return levelLayout;
}

sf::Texture& LevelManager::getTilesheet() {
    return tilesheet;
}

const LayerInfo LevelManager::getLayerInfo(int layerNo) const{
    return layers[layerNo];
}