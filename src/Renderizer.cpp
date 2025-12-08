#include "Renderizer.hpp"
#include "SFML/System/Vector2.hpp"

Renderizer::Renderizer(const RenderizerParameters& params)
: window(params.window), texture(params.texture), rect(params.rect) {
    sprite.setTexture(this->texture);
    sprite.setTextureRect(this->rect);
}

void Renderizer::render(sf::Vector2f position) {
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
    sprite.setPosition(position);
    window.draw(sprite);
}

