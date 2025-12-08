#pragma once

#include <vector>
#include "SFML/System/Vector2.hpp"

class BasicCollider{
public:
    BasicCollider() = default;
    void calculateCollisionGrid(sf::Vector2f position);
    bool isColliding(sf::Vector2f position1, sf::Vector2f size1, sf::Vector2f position2, sf::Vector2f size2);
    bool isCollidingWithLevel(sf::Vector2f position, int i);
    bool horizontalLevelCollision(sf::Vector2f& position);
    bool verticalLevelCollision(sf::Vector2f& position);
    void setSize(sf::Vector2f newSize);
protected:
    std::vector<int> collisionGrid;
    sf::Vector2f positionLastFrame;
    int directionLastCollision = 1;
    sf::Vector2f size;
};