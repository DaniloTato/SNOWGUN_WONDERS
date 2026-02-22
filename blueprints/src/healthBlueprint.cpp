#include "healthBlueprint.hpp"

#include "CollectableManager.hpp"
#include "GameState.hpp"
#include "healthPickup.hpp"

namespace blueprint {

TangibleObject *healthBlueprint(const sf::Vector2f &pos) {

  const std::string id = "health";

  RenderizerParameters params{
      .window = *GameState::getInstance().getMainWindow(),
      .texture = CollectableManager::getInstance().textureCache.get(id),
      .rect = {0, 0, 16, 16},
      .position = pos,
      .camera = GameState::getInstance().getMainCamera(),
      .layer = 0.f,
      .parallax = 1.f};

  auto *hp = new TangibleObject(
      params, CollectableManager::getInstance().animationCache.get(id));

  hp->collider.setSize({16.f, 16.f});
  hp->animator.play("idle");

  hp->scripter.addScript("behaviour", script::healthPickup);

  return hp;
}

} // namespace blueprint