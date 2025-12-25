#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "UISlider.hpp"
#include "LevelManager.hpp"

enum class PickerMode {
    Tiles,
    Enemies
};

struct PickerSelection {
    PickerMode mode = PickerMode::Tiles;
    sf::IntRect tileRect;
    std::string enemyId;
};

class TilePicker
{
public:
    TilePicker(sf::Texture& tileset, int tileSize);

    PickerSelection open(std::vector<LayerInfo>& layers, int& activeLayer);

private:
    void drawTileset(sf::RenderWindow& window, const sf::IntRect& selectedRect);

    void drawLayerList(
        sf::RenderWindow& window,
        std::vector<LayerInfo>& layers,
        int& activeLayer,
        sf::Font& font,
        const sf::Event& ev
    );

    void drawParallaxUI(
        sf::RenderWindow& window,
        LayerInfo& layer,
        sf::Font& font,
        UISlider& slider
    );

    void drawModeTabs(sf::RenderWindow& window, sf::Font& font, const sf::Event& ev);
    void drawEnemyPicker(sf::RenderWindow& window, sf::Font& font, const sf::Event& ev);

private:
    sf::Texture& tileset;
    int tileSize;

    PickerSelection selection;

    bool dragging = false;
    sf::Vector2i dragStart;
    sf::IntRect selectedRect;
};