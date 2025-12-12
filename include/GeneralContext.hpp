#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/System/Vector2.hpp>
#include "TangibleObject.hpp"
#include "RenderizerParameters.hpp"

struct GeneralContext {
    sf::Vector2f playerPosition = {0.f, 0.f};
    sf::RenderWindow& window;
    RenderizerParameters& textParams;
    TangibleObject* player;
};