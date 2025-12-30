#include "Animator.hpp"
#include "chest.hpp"

#include "CollectableManager.hpp"
#include "GameState.hpp"
#include "chest.hpp"

namespace blueprint {

TangibleObject *chestBlueprint(const sf::Vector2f &pos) {

  const std::string id = "chest";

  RenderizerParameters params{
      *GameState::getInstance().getMainWindow(),
      CollectableManager::getInstance().textureCache.get(id),
      {0, 0, 16, 16},
      pos,
      GameState::getInstance().getMainCamera(),
      0.f,
      1.f};

  auto *chest = new TangibleObject(
      params, CollectableManager::getInstance().animationCache.get(id));

  chest->collider.setSize({14.f, 10.f});
  chest->collider.setOffset({1.f, 6.f});

  chest->animator.play("closed");

  chest->scripter.addScript(script::chest);

  return chest;
}

} // namespace blueprint