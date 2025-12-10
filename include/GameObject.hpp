#pragma once
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "GeneralContext.hpp"

class GameObject {
public:
    GameObject(sf::Vector2f pos = {0.f, 0.f});
    virtual ~GameObject();
    virtual void update(const GeneralContext& ctx) = 0;

    static std::vector<GameObject*>& getGameObjects();
    static void destroy(GameObject* g);
    
    sf::Vector2f position;

protected:
    static std::vector<GameObject*> s_gameObjects;
};