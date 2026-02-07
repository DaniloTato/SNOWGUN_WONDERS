#include "crystalBlueprint.hpp"

#include "CollectableManager.hpp"
#include "GameState.hpp"
#include "crystalShard.hpp"

namespace blueprint {

TangibleObject *crystalBlueprint(const sf::Vector2f &pos) {

  const std::string id = "crystal";

  RenderizerParameters params{
      .window = *GameState::getInstance().getMainWindow(),
      .texture = CollectableManager::getInstance().textureCache.get(id),
      .rect = {0, 0, 16, 16},
      .position = pos,
      .camera = GameState::getInstance().getMainCamera(),
      .layer = 0.f,
      .parallax = 1.f};

  auto *crystal = new TangibleObject(
      params, CollectableManager::getInstance().animationCache.get(id));

  crystal->collider.setSize({12.f, 12.f});
  crystal->collider.setOffset({2.f, 2.f});

  crystal->animator.play("big");

  crystal->scripter.addScript("crystalShard", script::crystalShard);

  return crystal;
}
} // namespace blueprint