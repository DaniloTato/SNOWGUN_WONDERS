#include "GameObject.hpp"
#include "GameObjectExposure.hpp"
#include "SFML/System/Vector2.hpp"
#include <vector>

unsigned int GameObject::nextId = 0;

GameObject::GameObject(sf::Vector2f pos) : position(pos), exposesId(true) {
  s_gameObjects.push_back(this);
  id = nextId++;
}

GameObject::~GameObject() {
  auto it = std::ranges::find(s_gameObjects, this);
  if (it != s_gameObjects.end()) {
    s_gameObjects.erase(it);
  }
}

void GameObject::destroy(GameObject *g) {
  std::vector<GameObject *> &list = GameObject::s_gameObjects;

  // Interesting use of if statement with an initizalizer. C++17

  /*auto it = std::ranges::find(list, g); — This runs first. It creates the
  variable it.

  Second ExprStmt is the boolean that determines if the body of the if is
  executed. */

  if (auto it = std::ranges::find(list, g); it != list.end()) {
    *it = list.back();
    list.pop_back();
  }

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

unsigned int GameObject::getId() { return id; }

void GameObject::setExposesId(bool value) { exposesId = value; }

bool GameObject::getExposesId() const { return exposesId; }

void getTerminalObject() {}

GameObjectExposure::Value::Object GameObject::describe() {

  auto desc = std::make_shared<GameObjectExposure::Descriptor>();

  desc->fields["pos"] =
      std::make_shared<GameObjectExposure::Field>(GameObjectExposure::Field{
          .getValue =
              [this]() {
                return GameObjectExposure::Value{
                    GameObjectExposure::Descriptor::describeVector2f(position)};
              },
          .setValue =
              [](const GameObjectExposure::Value &) {
                // optional: disallow replacing the whole vector
              }});

  desc->fields["offset"] =
      std::make_shared<GameObjectExposure::Field>(GameObjectExposure::Field{
          .getValue =
              [this]() {
                return GameObjectExposure::Value{
                    GameObjectExposure::Descriptor::describeVector2f(offset)};
              },
          .setValue =
              [](const GameObjectExposure::Value &) {
                // optional
              }});

  return desc;
}