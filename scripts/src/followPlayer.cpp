
#include "followPlayer.hpp"

#include "Constants.hpp"
#include "GameCamera.hpp"
#include "LevelManager.hpp"
#include "SceneManager.hpp"

namespace script {

void followPlayer(GameCamera &camera, const GeneralContext &ctx) {
  sf::Vector2f playerPosition = *ctx.playerPosition;

  float zoom = camera.getZoom();
  sf::Vector2f halfViewSize(Constants::SCREEN_WIDTH / 2.f / zoom,
                            Constants::SCREEN_HEIGHT / 2.f / zoom);
  sf::Vector2f desiredCamPos =
      playerPosition -
      (halfViewSize - LevelManager::getInstance().getCameraPlayerRelation());

  if (desiredCamPos.x > 3000 && desiredCamPos.y > 87 * 16) {
    desiredCamPos.y = 87 * 16;
    if (playerPosition.y >
        desiredCamPos.y + Constants::SCREEN_HEIGHT / camera.getZoom()) {
      SceneManager::getInstance().reloadCurrentScene();
    }
  }

  camera.goTo(desiredCamPos);
}

} // namespace script