#pragma once
#include "GameObjectExposure.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

struct GeneralContext;

class GameObject {
public:
  GameObject(sf::Vector2f pos = {0.f, 0.f});
  virtual ~GameObject();
  virtual void update(const GeneralContext &ctx) = 0;

  void makePersistentAcrossScenes();

  virtual std::shared_ptr<GameObjectExposure::Descriptor> describe();
  unsigned int getId();

  static std::vector<GameObject *> &getGameObjects();
  static void destroy(GameObject *g);

  static void destroySceneObjects();

  sf::Vector2f position;
  sf::Vector2f offset;

protected:
  bool persistentAcrossScenes = false;
  static std::vector<GameObject *> s_gameObjects;

private:
  unsigned int id;
  static unsigned int nextId;
};