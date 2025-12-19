#pragma once

#include <SFML/System/Vector2.hpp>
#include "TangibleObject.hpp"
#include "RenderizerParameters.hpp"

struct GeneralContext {
    sf::Vector2f *playerPosition;
    RenderizerParameters* textParams;
    TangibleObject* player;
    sf::Texture* bulletTexture;
};