#include "Renderizer.hpp"
#include "SFML/System/Vector2.hpp"

#include <iostream>

std::vector<RenderEntry> Renderizer::registry;

Renderizer::Renderizer(const RenderizerParameters& params)
: window(params.window), texture(params.texture), rect(params.rect), assignedCamera(params.camera), layer(params.layer), paralax(params.parallax) {
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
    
}

Renderizer::~Renderizer(){
    unregisterPair(this);
}

void Renderizer::registerPair(GameObject* obj, Renderizer* rend){
    registry.push_back({ obj, rend });
}

void Renderizer::unregisterPair(Renderizer* rend){
    registry.erase(
        std::remove_if(registry.begin(), registry.end(),
                       [&](const RenderEntry& e){ return e.renderer == rend; }),
        registry.end()
    );
}

void Renderizer::assignCamera(GameCamera* cam) {
    assignedCamera = cam;
}

void Renderizer::render(GameObject* obj) {

    sf::Vector2f position = obj->position;

    if (!assignedCamera) {
        sprite.setPosition(position);
        sprite.setScale(1.f, 1.f);
    } else {
        sf::Vector2f screenPos = assignedCamera->worldToScreen(position, paralax);
        sprite.setPosition(screenPos);
        sprite.setScale(assignedCamera->getZoom(), assignedCamera->getZoom());
    }
    window.draw(sprite);
}

const float Renderizer::getLayer() const{
    return layer;
}

void Renderizer::setRect(const sf::IntRect& newRect, int direction) {
    sf::IntRect directionRect = newRect;
    directionRect.left = newRect.left + directionRect.width * ((direction - 1)/-2);
    directionRect.width = newRect.width * direction;
    sprite.setTextureRect(directionRect);
}

void Renderizer::renderAll(){
    std::stable_sort(registry.begin(), registry.end(),
        [](const RenderEntry& a, const RenderEntry& b) {
            return a.renderer->getLayer() > b.renderer->getLayer();
        }
    );

    for (auto& e : registry)
        e.renderer->render(e.object);
}

const sf::IntRect& Renderizer::getRect() const {
    return rect;
}