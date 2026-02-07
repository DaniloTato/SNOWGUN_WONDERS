#include "reindeer.hpp"

#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "reindeerAI.hpp"

namespace blueprint {

TangibleObject *reindeer(const sf::Vector2f &pos) {

  const std::string id = "reindeer";

  RenderizerParameters params{
      .window = *GameState::getInstance().getMainWindow(),
      .texture = EnemyManager::getInstance().textureCache.get(id),
      .rect{0, 0, 32, 16},
      .position = pos,
      .camera = GameState::getInstance().getMainCamera(),
      .layer = 0.f,
      .parallax = 1.f};

  auto *heli = new TangibleObject(
      params, EnemyManager::getInstance().animationCache.get(id));

  heli->collider.setOffset({4.f, 4.f});
  heli->collider.setSize({24.f, 8.f});

  heli->animator.play("fly");

  heli->scripter.addScript("reindeerAI", script::reindeerAI);

  return heli;
}

} // namespace blueprint