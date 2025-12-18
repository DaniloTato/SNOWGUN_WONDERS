#pragma once

#include <SFML/Graphics.hpp>

namespace ColorPalette {
    inline const sf::Color ElectricBlue = sf::Color(0,0,252);
    inline const sf::Color LimeGreen = sf::Color(184,248,24);
    inline const sf::Color Cyan = sf::Color(0,252,252);
    inline const sf::Color HotPink = sf::Color(248,120,248);
    inline const sf::Color SoftViolet = sf::Color(152, 120, 248);
    inline const sf::Color MexicanPink = sf::Color(228, 0, 88);
    inline const sf::Color Black = sf::Color(0,0,0);
    inline const sf::Color White = sf::Color(252,252,252);
    inline const sf::Color DarkCyanBlue = sf::Color(0,64,88);

    inline constexpr std::array<const sf::Color*, 6> EXPLOSION_COLORS = {
        &HotPink,
        &SoftViolet,
        &LimeGreen,
        &Cyan,
        &White,
        &MexicanPink,
    };
}