#include "Renderizer.hpp"
#include "SFML/System/Vector2.hpp"

Renderizer::Renderizer(const RenderizerParameters& params)
: window(params.window), texture(params.texture), rect(params.rect), assignedCamera(params.camera) {
    sprite.setTexture(this->texture);
    sprite.setTextureRect(this->rect);
}

void Renderizer::assignCamera(GameCamera* cam) {
    assignedCamera = cam;
}

void Renderizer::render(sf::Vector2f position) {
    if (!assignedCamera) {
        sprite.setPosition(position);
        sprite.setScale(1.f, 1.f);
    } else {
        sf::Vector2f screenPos = assignedCamera->worldToScreen(position);
        sprite.setPosition(screenPos);
        sprite.setScale(assignedCamera->getZoom(), assignedCamera->getZoom());
    }
    window.draw(sprite);
}

