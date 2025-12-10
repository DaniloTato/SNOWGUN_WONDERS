#pragma once
#include "GameObject.hpp"
#include "SFML/System/Vector2.hpp"
#include "Scripter.hpp"
#include "GeneralContext.hpp"
#include <SFML/Graphics.hpp>

class GameCamera: public GameObject {
public:
    GameCamera();

    const sf::Vector2f& getPosition() const;

    void update(const GeneralContext& ctx) override;

    void goTo(const sf::Vector2f& pos);
    void zoomTo(float desiredZoom);
    float getZoom() const;
    const float getDesiredZoom() const;
    void setCameraShakePosition(const sf::Vector2f& shakePos);
    void setImpactZoom(float impactZoom);

    const sf::Vector2f screenToWorld(const sf::Vector2f& screenPos, float parallax) const;
    const sf::Vector2f worldToScreen(const sf::Vector2f& worldPos, float parallax = 1.0f) const;

    Scripter<GameCamera> scripter;

private:
    sf::Vector2f desiredPosition;
    sf::Vector2f shakePosition;
    float zoom;
    float desiredZoom;
    float impactZoom;
    float speed;
};