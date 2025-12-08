#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>

struct RenderizerParameters {
    sf::RenderWindow& window;
    sf::Texture& texture;
    sf::IntRect& rect;
    sf::Vector2f position = {0.f, 0.f};
};

class Renderizer {
public:
    Renderizer(const RenderizerParameters& params);
    ~Renderizer() = default;

    void render(sf::Vector2f position);

private:
    sf::RenderWindow& window;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect rect;
};