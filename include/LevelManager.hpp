#pragma once
#include <vector>
#include <string>
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "GameCamera.hpp"
#include "RenderableObject.hpp"
#include <nlohmann/json.hpp>

struct TileCreationRequest {
    sf::RenderWindow& window;
    GameCamera* camera;
    int layer;
    int x;
    int y;
    sf::IntRect rect;
};

struct TileDeletionRequest {
    int layer;
    int x;
    int y;
};

struct TileInfo {
    int x, y;
    sf::IntRect textureRect;
    RenderableObject* object = nullptr;
};

struct LayerInfo {
    float paralax = 1.f;
    std::string name = "default";
    std::vector<TileInfo> tiles;
};

class LevelManager {
public:
    static LevelManager& getInstance();

    void loadLevel(sf::RenderWindow& window, GameCamera* camera, const std::string& path);
    void loadLayer(sf::RenderWindow& window,
                   GameCamera* camera,
                   int layerNo,
                   const nlohmann::json& layerJSON,
                   int tileSize);

    void createTile(sf::RenderWindow& window, GameCamera* camera, int layerNo,
                    int x, int y, sf::IntRect rect);

    void deleteTile(int layerNo, int x, int y);
    void saveLevel(const std::string& path);

    void deleteLayerObjects(int layerNo);
    void reloadAllLayers(sf::RenderWindow& window, GameCamera* camera);
    void reloadLayer(sf::RenderWindow& window, GameCamera* camera, int layerNo);

    void queueCreateTile(sf::RenderWindow& window, GameCamera* camera, int layer, int x, int y, const sf::IntRect& rect);
    void queueDeleteTile(int layer, int x, int y);
    void applyQueuedTileChanges();

    const LayerInfo getLayerInfo(int layerNo) const;

    const std::vector<std::vector<int>>& getLevelLayout() const;

    sf::Texture& getTilesheet();
    std::vector<LayerInfo> layers;
    int activeLayer;

private:
    LevelManager();

    sf::Texture tilesheet;

    std::vector<std::vector<int>> levelLayout;
    std::vector<TileCreationRequest> createQueue;
    std::vector<TileDeletionRequest> deleteQueue;

    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;

    LevelManager(LevelManager&&) = delete;
    LevelManager& operator=(LevelManager&&) = delete;
};