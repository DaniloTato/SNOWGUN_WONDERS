#include "crystalBlueprint.hpp"

#include "CollectableManager.hpp"
#include "GameState.hpp"
#include "crystalShard.hpp"

namespace blueprint {

TangibleObject *crystalBlueprint(const sf::Vector2f &pos) {

  const std::string id = "crystal";

  RenderizerParameters params{
      *GameState::getInstance().getMainWindow(),
      CollectableManager::getInstance().textureCache.get(id),
      {0, 0, 16, 16},
      pos,
      GameState::getInstance().getMainCamera(),
      0.f,
      1.f};

  auto *crystal = new TangibleObject(
      params, CollectableManager::getInstance().animationCache.get(id));

  crystal->collider.setSize({12.f, 12.f});
  crystal->collider.setOffset({2.f, 2.f});

  crystal->animator.play("big");

  crystal->scripter.addScript(script::crystalShard);

  return crystal;
}
} // namespace blueprint