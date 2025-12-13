#pragma once
#include <SFML/System/Vector2.hpp>

class PhysicsComponent {
public:
    PhysicsComponent();
    PhysicsComponent(float gravity);

    void updateX(sf::Vector2f& position);
    void updateY(sf::Vector2f& position);

    const sf::Vector2f& getSpeed() const { return speed; }
    float getSpdx() const { return speed.x; }
    float getSpdy() const { return speed.y; }

    void setSpeed(const sf::Vector2f& newSpeed) { speed = newSpeed; }
    void setSpdx(float x) { speed.x = x; }
    void setSpdy(float y) { speed.y = y; }
    void setXFriction(float newFriction) { xFriction = newFriction; }

    float getGravity() const { return gravity; }
    void setGravity(float g) { gravity = g; }

    void turnOffFriction();

private:
    sf::Vector2f speed;
    float gravity;
    float xFriction;
    bool hasFriction;
};