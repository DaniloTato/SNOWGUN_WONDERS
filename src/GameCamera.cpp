#include "GameCamera.hpp"
#include "Constants.hpp"
#include "GameObject.hpp"
#include "SFML/System/Vector2.hpp"
#include "Scripter.hpp"

sf::Vector2i Vec2fTo2i(sf::Vector2f vec){
    return {static_cast<int>(vec.x), static_cast<int>(vec.y)};
}

sf::Vector2f Vec2iTo2f(sf::Vector2i vec){
    return {static_cast<float>(vec.x), static_cast<float>(vec.y)};
}

GameCamera::GameCamera()
    : GameObject({0,0})
    , desiredPosition(0.f, 0.f)
    , shakePosition(0.f, 0.f)
    , zoom(1.f)
    , desiredZoom(1.f)
    , impactZoom(0.f)
    , followSpeed(sf::Vector2f(0.1,0.05))
    , zoomSpeed(0.3)
{}

void GameCamera::goTo(const sf::Vector2f& pos) {
    desiredPosition = pos;
}

void GameCamera::zoomTo(float dZoom) {
    desiredZoom = dZoom;
}

void GameCamera::update(const GeneralContext& ctx) {
    scripter.runScripts(*this, ctx);
    zoom += ((desiredZoom + impactZoom) - zoom) * zoomSpeed;
    position.x += ((desiredPosition.x + shakePosition.x) - position.x) * followSpeed.x;
    position.y += ((desiredPosition.y + shakePosition.y) - position.y) * followSpeed.y;
}

float GameCamera::getZoom() const {
    return zoom;
}

const float GameCamera::getDesiredZoom() const {
    return desiredZoom;
}

const sf::Vector2f GameCamera::worldToScreen(const sf::Vector2f& worldPos, float parallax) const {
    if (parallax <= 0.f) parallax = 1.f;

    sf::Vector2f base = worldPos - (position / parallax);

    sf::Vector2f zoomOffset = (zoom - 1.f) * base;

    float parallaxOffsetX = ((-2 + Constants::SCREEN_WIDTH * (-1 + parallax)) / (2 * parallax) + 1);
    float parallaxOffsetY = ((-2 + Constants::SCREEN_HEIGHT * (-1 + parallax)) / (2 * parallax) + 1);

    sf::Vector2f parallaxOffset(parallaxOffsetX, parallaxOffsetY);

    return Vec2iTo2f(Vec2fTo2i(base + zoomOffset + parallaxOffset));
}

const sf::Vector2f GameCamera::screenToWorld(const sf::Vector2f& screenPos, float parallax) const{
    if (parallax <= 0.f) parallax = 1.f;

    float parallaxOffsetX = ((-2 + Constants::SCREEN_WIDTH * (-1 + parallax)) / (2 * parallax) + 1);
    float parallaxOffsetY = ((-2 + Constants::SCREEN_HEIGHT * (-1 + parallax)) / (2 * parallax) + 1);
    sf::Vector2f parallaxOffset(parallaxOffsetX, parallaxOffsetY);

    sf::Vector2f base = (screenPos - parallaxOffset) / zoom;

    sf::Vector2f world = base + (position / parallax);

    return world;
}

sf::FloatRect GameCamera::getWorldViewRect() const {
    sf::Vector2f size = {Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT};
    sf::Vector2f topLeft = {position.x - (size.x * 0.5f), position.y - (size.y * 0.5f)};
    return sf::FloatRect(topLeft, size);
}

void GameCamera::setCameraShakePosition(const sf::Vector2f& shakePos) {
    shakePosition = shakePos;
}

void GameCamera::setImpactZoom(float impactZoom) {
    this->impactZoom = impactZoom;
}