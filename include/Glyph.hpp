#pragma once

#include <SFML/Graphics.hpp>

struct Glyph {
    char c;
    sf::IntRect texRect;
    sf::Vector2f basePos;
    sf::Color color;
    enum Anim { None, Sin, Shake } anim = None;
    float animParam = 0.f; // for shake intensity or sin amplitude/freq if needed
    float phase = 0.f;     // per-glyph phase for wave offset
    int appearIndex = -1;  // index order for typewriter reveal
};

struct RenderGlyph{
    sf::IntRect rect;
    sf::Vector2f pos;
    sf::Color color;
};