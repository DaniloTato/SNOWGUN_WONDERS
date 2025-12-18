#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

class PhysicsComponent {
public:

    enum class SpeedType{
        MOVEMENT,
        KICK,
        COUNT
    };

    PhysicsComponent();
    PhysicsComponent(float gravity);

    void updateX(sf::Vector2f& position);
    void updateY(sf::Vector2f& position);

    const sf::Vector2f& getSpeed(SpeedType type) const;
    float getSpdx(SpeedType type) const;
    float getSpdy(SpeedType type) const;

    void setSpeed(const sf::Vector2f& newSpeed, SpeedType type);
    void setSpdx(float x, SpeedType type);
    void setSpdy(float y, SpeedType type);

    void turnOffFriction();

    float gravity;
    float xFriction;

private:
    std::vector<sf::Vector2f> speeds;
    bool hasFriction;
};