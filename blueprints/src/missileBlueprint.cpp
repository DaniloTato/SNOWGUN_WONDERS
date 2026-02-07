#include "missileBlueprint.hpp"

#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "missileAI.hpp"

namespace blueprint {

TangibleObject *missileBlueprint(const sf::Vector2f &pos) {

  const std::string id = "missile";

  RenderizerParameters params{
      .window = *GameState::getInstance().getMainWindow(),
      .texture = EnemyManager::getInstance().textureCache.get(id),
      .rect = {0, 0, 48, 48},
      .position = pos,
      .camera = GameState::getInstance().getMainCamera(),
      .layer = -11.f,
      .parallax = 1.f};

  auto *missile = new TangibleObject(
      params, EnemyManager::getInstance().animationCache.get(id));

  missile->collider.setOffset({0.f, 16.f});
  missile->collider.setSize({32.f, 32.f});

  missile->animator.play("idle");

  missile->scripter.addScript("missileAI", script::missileAI);
  missile->direction = -1;

  return missile;
}

} // namespace blueprint