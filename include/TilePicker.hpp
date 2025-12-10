#pragma once
#include "SFML/Graphics/Rect.hpp"
#include <SFML/Graphics.hpp>

class TilePicker {
public:
    TilePicker(
        sf::Texture& tileset,
        int tileSize
    );

    sf::IntRect open();

private:
    sf::Texture& tileset;
    int tileSize;
};