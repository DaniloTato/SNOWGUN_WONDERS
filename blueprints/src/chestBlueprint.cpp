#include "Animator.hpp"
#include "chest.hpp"

#include "CollectableManager.hpp"
#include "GameState.hpp"
#include "chest.hpp"

namespace blueprint {

TangibleObject *chestBlueprint(const sf::Vector2f &pos) {

  const std::string id = "chest";

  RenderizerParameters params{
      .window = *GameState::getInstance().getMainWindow(),
      .texture = CollectableManager::getInstance().textureCache.get(id),
      .rect = {0, 0, 16, 16},
      .position = pos,
      .camera = GameState::getInstance().getMainCamera(),
      .layer = 0.f,
      .parallax = 1.f};

  auto *chest = new TangibleObject(
      params, CollectableManager::getInstance().animationCache.get(id));

  chest->collider.setSize({14.f, 10.f});
  chest->collider.setOffset({1.f, 6.f});

  chest->animator.play("closed");

  chest->scripter.addScript("behaviour", script::chest);

  return chest;
}

} // namespace blueprint