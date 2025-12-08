#include "GameObject.hpp"
#include "SFML/System/Vector2.hpp"
#include <vector>

GameObject::GameObject(sf::Vector2f pos) : position(pos) {
    s_gameObjects.push_back(this);
}

GameObject::~GameObject() {
    auto it = std::find(s_gameObjects.begin(), s_gameObjects.end(), this);
    if (it != s_gameObjects.end()) {
        s_gameObjects.erase(it);
    }
}

std::vector<GameObject*> GameObject::s_gameObjects;

std::vector<GameObject*>& GameObject::getGameObjects() {
    return s_gameObjects;
}