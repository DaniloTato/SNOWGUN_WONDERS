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

void GameObject::destroy(GameObject *g) {
  auto &list = GameObject::s_gameObjects;
  list.erase(std::remove(list.begin(), list.end(), g), list.end());
  delete g;
}

std::vector<GameObject *> GameObject::s_gameObjects;

std::vector<GameObject *> &GameObject::getGameObjects() {
  return s_gameObjects;
}

void GameObject::destroySceneObjects() {
  auto &objects = getGameObjects();

  for (int i = static_cast<int>(objects.size()) - 1; i >= 0; --i) {
    GameObject *obj = objects[i];

    if (!obj->persistentAcrossScenes) {
      destroy(obj);
    }
  }
}

void GameObject::makePersistentAcrossScenes() { persistentAcrossScenes = true; }

void getTerminalObject() {}

GameObjectDescriptor GameObject::describe() const {
  GameObjectDescriptor d;
  d.fields["position"] = GameObjectDescriptor::describeVector2f(position);
  d.fields["hellooooo"] = ":)";
  return d;
}