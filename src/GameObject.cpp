#include "GameObject.hpp"
#include "SFML/System/Vector2.hpp"
#include <vector>
#include <typeinfo>
#include <iostream>

GameObject::GameObject(sf::Vector2f pos)
: position(pos){
    s_gameObjects.push_back(this);
}

GameObject::~GameObject() {
    auto it = std::find(s_gameObjects.begin(), s_gameObjects.end(), this);
    if (it != s_gameObjects.end()) {
        s_gameObjects.erase(it);
    }
}

void GameObject::destroy(GameObject* g){
    auto& list = GameObject::s_gameObjects;
    list.erase(std::remove(list.begin(), list.end(), g), list.end());
    delete g;
}

std::vector<GameObject*> GameObject::s_gameObjects;

std::vector<GameObject*>& GameObject::getGameObjects() {
    return s_gameObjects;
}

void GameObject::destroySceneObjects() {
    auto& objects = getGameObjects();

    std::cout << "Destroying scene objects. Total: " << objects.size() << "\n";

    // Use a reverse loop to safely remove while iterating
    for (int i = static_cast<int>(objects.size()) - 1; i >= 0; --i) {
        GameObject* obj = objects[i];

        if (!obj->persistentAcrossScenes) {
            // Print pointer and RTTI type info if available
            std::cout << "Destroying object at " << obj
                      << ", type: " << typeid(*obj).name() << "\n";

            try {
                destroy(obj);  // Your existing destroy function
            } catch (const std::exception& e) {
                std::cerr << "Exception destroying object at " << obj
                          << ": " << e.what() << "\n";
            } catch (...) {
                std::cerr << "Unknown exception destroying object at " << obj << "\n";
            }
        } else {
            std::cout << "Skipping persistent object at " << obj
                      << ", type: " << typeid(*obj).name() << "\n";
        }
    }

    std::cout << "Finished destroying scene objects.\n";
}