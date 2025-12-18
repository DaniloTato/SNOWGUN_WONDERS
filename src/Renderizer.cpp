#include "Renderizer.hpp"
#include "GameState.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

std::vector<RenderEntry> Renderizer::registry;

Renderizer::Renderizer(const RenderizerParameters& params)
: window(params.window),
texture(params.texture), 
rect(params.rect),
color(sf::Color::White), 
assignedCamera(params.camera), 
layer(params.layer), 
paralax(params.parallax), 
show(true), 
showCountDown(0.f)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
    
}

Renderizer::~Renderizer(){
    unregisterPair(this);
}

void Renderizer::registerPair(GameObject* obj, Renderizer* rend, bool isRectShape){
    registry.push_back({ obj, rend, isRectShape});
}

void Renderizer::unregisterPair(Renderizer* rend){
    registry.erase(
        std::remove_if(registry.begin(), registry.end(),
                       [&](const RenderEntry& e){ return e.renderizer == rend; }),
        registry.end()
    );
}

void Renderizer::assignCamera(GameCamera* cam) {
    assignedCamera = cam;
}

void Renderizer::render(GameObject* obj) {

    sf::Vector2f position = obj->position + obj->offset;

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

void Renderizer::renderRectShape(GameObject* obj) {

    sf::Vector2f position = obj->position + obj->offset;

    sf::RectangleShape rectShape;
    rectShape.setFillColor(color);

    if (!assignedCamera) {
        rectShape.setPosition(position);
        rectShape.setSize(static_cast<sf::Vector2f>(rect.getSize()));
    } else{
        sf::Vector2f screenPos = assignedCamera->worldToScreen(position, paralax);
        sprite.setPosition(screenPos);
        rectShape.setSize(static_cast<sf::Vector2f>(rect.getSize()) * assignedCamera->getZoom());
    }

    window.draw(sprite);
}

void Renderizer::setColor(sf::Color newColor){
    color = newColor;
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
            return a.renderizer->getLayer() > b.renderizer->getLayer();
        }
    );

    for (auto& entry : registry)
        if(entry.renderizer -> shouldIRender()){
            entry.renderizer->render(entry.object);
        }
}

const sf::IntRect& Renderizer::getRect() const {
    return rect;
}

void Renderizer::toggleShowEvery(float time){
    showCountDown -= GameState::getInstance().dt();
    if(showCountDown <= 0){
        show = !show;
        showCountDown = time;
    }
}

bool Renderizer::shouldIRender(){
    return show;
}