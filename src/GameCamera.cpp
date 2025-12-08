#include "GameCamera.hpp"
#include "Constants.hpp"

GameCamera::GameCamera()
    : position(0.f, 0.f), desiredPosition(0.f, 0.f), zoom(1.f), desiredZoom(1.f), speed(0.1f) {}

GameCamera::~GameCamera() {
    scripts.clear();
}

void GameCamera::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

const sf::Vector2f& GameCamera::getPosition() const {
    return position;
}

void GameCamera::goTo(const sf::Vector2f& pos) {
    desiredPosition = pos;
}

void GameCamera::zoomTo(float dZoom) {
    desiredZoom = dZoom;
}

void GameCamera::update(const CameraContext& ctx) {
    if(!scripts.empty()) {
        for (auto& script : scripts) {
            if(script){
                script(*this, ctx);
            }
        }
    }

    zoom += (desiredZoom - zoom) * speed;
    position += (desiredPosition - position) * speed;
}

float GameCamera::getZoom() const {
    return zoom;
}

const float GameCamera::getDesiredZoom() const {
    return desiredZoom;
}

void GameCamera::addScript(void (*func)(GameCamera&, const CameraContext&)) {
    scripts.push_back(func);
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