#pragma once
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class GameObject {
public:
    GameObject(sf::Vector2f pos = {0.f, 0.f});
    ~GameObject();
    virtual void update() = 0;

    static std::vector<GameObject*>& getGameObjects();
    const sf::Vector2f getPosition() const;

protected:
    sf::Vector2f position;
    static std::vector<GameObject*> s_gameObjects;
};