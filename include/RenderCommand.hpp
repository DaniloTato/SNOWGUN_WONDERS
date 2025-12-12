#pragma once

#include <SFML/Graphics.hpp>

struct RenderCommand{
    sf::IntRect rect;
    sf::Vector2f pos;
    sf::Color color;
};