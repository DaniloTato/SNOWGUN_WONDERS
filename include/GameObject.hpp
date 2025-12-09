#pragma once
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "ContextTypes.hpp"

class GameObject {
public:
    GameObject(sf::Vector2f pos = {0.f, 0.f});
    ~GameObject();
    virtual void update(const GameContext& ctx) = 0;

    static std::vector<GameObject*>& getGameObjects();
    
    sf::Vector2f position;

protected:
    static std::vector<GameObject*> s_gameObjects;
};