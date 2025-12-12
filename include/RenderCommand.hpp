#pragma once

#include <limits>
#include <SFML/Graphics.hpp>

struct RenderCommand{
    sf::IntRect rect;
    sf::Vector2f pos;
    sf::Color color;
    float overrideParalax = std::numeric_limits<float>::max();
};