#include "toy.hpp"

#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "enemyPatrol.hpp"

namespace blueprint {

TangibleObject *toy(const sf::Vector2f &pos) {

  const std::string id = "toy";

  RenderizerParameters params{
      .window = *GameState::getInstance().getMainWindow(),
      .texture = EnemyManager::getInstance().textureCache.get(id),
      .rect = {0, 0, 17, 17},
      .position = pos,
      .camera = GameState::getInstance().getMainCamera(),
      .layer = 0.f,
      .parallax = 1.f};

  auto *toy = new TangibleObject(
      params, EnemyManager::getInstance().animationCache.get(id));

  toy->collider.setOffset({5.f, 2.f});
  toy->collider.setSize({15.f, 14.f});

  toy->animator.play("walking");

  toy->scripter.addScript("enemyPatrol", script::enemyPatrol);
  toy->direction = -1;

  return toy;
}

} // namespace blueprint