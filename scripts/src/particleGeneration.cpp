#include "particleGeneration.hpp"
#include "Constants.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "ParticleManager.hpp"

#include "SFML/System/Vector2.hpp"
#include "ScriptRunner.hpp"

namespace script {

void particleGeneration(ScriptRunner &runner, const GeneralContext &ctx) {

  // Particles are not game objects. Hence, they do not need to be queued to be
  // destroyed.

  static bool prewarmed = false;

  const float snowSpawnRange = 7.f;
  auto *camera = GameState::getInstance().getMainCamera();
  if (!prewarmed) {
    for (int i = 0; i < 1200; i++) {
      sf::Vector2f worldPos = camera->screenToWorld(
          {static_cast<float>(Helper::randRange(
               -Constants::SCREEN_WIDTH * snowSpawnRange * 0.5f,
               Constants::SCREEN_WIDTH * snowSpawnRange * 0.5f)),
           static_cast<float>(Helper::randRange(
               -Constants::SCREEN_WIDTH * snowSpawnRange * 0.5f,
               Constants::SCREEN_WIDTH * snowSpawnRange * 0.5f))},
          1.f);

      if (worldPos.x <= 370 * 16) {
        ParticleManager::getInstance().emitSnow(worldPos);
      }
    }

    prewarmed = true;
    return;
  }

  for (int i = 0; i < 3; i++) {
    sf::Vector2f inGamePosition = camera->screenToWorld(
        {static_cast<float>(Helper::randRange(
             -Constants::SCREEN_WIDTH * snowSpawnRange * 0.5f,
             Constants::SCREEN_WIDTH * snowSpawnRange * 0.5f)),
         -50.f},
        1.f);

    if (inGamePosition.x > 370 * 16)
      continue;

    ParticleManager::getInstance().emitSnow(inGamePosition);
  }
}
} // namespace script