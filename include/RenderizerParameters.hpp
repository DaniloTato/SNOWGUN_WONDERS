#pragma once

#include <SFML/Graphics.hpp>

class GameCamera;

struct RenderizerParameters {
    sf::RenderWindow& window;
    sf::Texture& texture;
    sf::IntRect rect;
    sf::Vector2f position = {0.f, 0.f};
    GameCamera* camera = nullptr;
    float layer = 0.f;
    float parallax = 1.f;
    bool registerAsRectShape = false;
};