#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "UISlider.hpp"
#include "LevelManager.hpp" 

class TilePicker
{
public:
    TilePicker(sf::Texture& tileset, int tileSize);

    sf::IntRect open(std::vector<LayerInfo>& layers, int& activeLayer);

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
        UISlider& slider,
        const sf::Event& ev
    );

private:
    sf::Texture& tileset;
    int tileSize;

    bool dragging = false;
    sf::Vector2i dragStart;
    sf::IntRect selectedRect;

    static float clamp01(float v) {
        return (v < 0.f ? 0.f : (v > 1.f ? 1.f : v));
    }
};