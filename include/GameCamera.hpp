#pragma once
#include "SFML/System/Vector2.hpp"
#include "Scripter.hpp"
#include <SFML/Graphics.hpp>

struct CameraContext{
    sf::Vector2f position = {0.f, 0.f};
};

class GameCamera {
public:
    GameCamera();
    ~GameCamera();

    void setPosition(const sf::Vector2f& pos);
    const sf::Vector2f& getPosition() const;

    void goTo(const sf::Vector2f& pos);
    void zoomTo(float desiredZoom);
    void update(const CameraContext& ctx);
    float getZoom() const;
    const float getDesiredZoom() const;
    void setCameraShakePosition(const sf::Vector2f& shakePos);
    void setImpactZoom(float impactZoom);

    sf::Vector2f worldToScreen(const sf::Vector2f& worldPos, float parallax = 1.0f) const;

    Scripter<GameCamera, CameraContext> scripter;

private:
    sf::Vector2f position;
    sf::Vector2f desiredPosition;
    sf::Vector2f shakePosition;
    float zoom;
    float desiredZoom;
    float impactZoom;
    float speed;
};