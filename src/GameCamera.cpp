#include "GameCamera.hpp"
#include "Constants.hpp"
#include "GameObject.hpp"
#include "Scripter.hpp"

GameCamera::GameCamera()
    : GameObject({0,0}), desiredPosition(0.f, 0.f), shakePosition(0.f, 0.f), zoom(1.f), desiredZoom(1.f), impactZoom(0.f), speed(0.1f) {}

void GameCamera::goTo(const sf::Vector2f& pos) {
    desiredPosition = pos;
}

void GameCamera::zoomTo(float dZoom) {
    desiredZoom = dZoom;
}

void GameCamera::update(const GeneralContext& ctx) {
    scripter.runScripts(*this, ctx);
    zoom += ((desiredZoom + impactZoom) - zoom) * speed;
    position += ((desiredPosition + shakePosition) - position) * speed;
}

float GameCamera::getZoom() const {
    return zoom;
}

const float GameCamera::getDesiredZoom() const {
    return desiredZoom;
}

sf::Vector2f GameCamera::worldToScreen(const sf::Vector2f& worldPos, float parallax) const {
    if (parallax <= 0.f) parallax = 1.f;

    sf::Vector2f base = worldPos - (position / parallax);

    sf::Vector2f zoomOffset = (zoom - 1.f) * base;

    float parallaxOffsetX = ((-2 + Constants::SCREEN_WIDTH * (-1 + parallax)) / (2 * parallax) + 1);
    float parallaxOffsetY = ((-2 + Constants::SCREEN_HEIGHT * (-1 + parallax)) / (2 * parallax) + 1);

    sf::Vector2f parallaxOffset(parallaxOffsetX, parallaxOffsetY);

    return base + zoomOffset + parallaxOffset;
}

void GameCamera::setCameraShakePosition(const sf::Vector2f& shakePos) {
    shakePosition = shakePos;
}

void GameCamera::setImpactZoom(float impactZoom) {
    this->impactZoom = impactZoom;
}