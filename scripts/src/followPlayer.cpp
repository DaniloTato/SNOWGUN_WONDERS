
#include "followPlayer.hpp"

#include "Constants.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include "LevelManager.hpp"

namespace script {

void followPlayer(GameCamera &camera, const GeneralContext &ctx) {
  sf::Vector2f playerPosition = *ctx.playerPosition;

  float zoom = camera.getZoom();
  sf::Vector2f halfViewSize(Constants::SCREEN_WIDTH / 2.f / zoom,
                            Constants::SCREEN_HEIGHT / 2.f / zoom);
  sf::Vector2f desiredCamPos =
      playerPosition -
      (halfViewSize - LevelManager::getInstance().getCameraPlayerRelation());

  camera.goTo(desiredCamPos);

  if (InputManager::getInstance().isJustPressed("zoomIn")) {
    GameState::getInstance().getMainCamera()->zoomTo(
        GameState::getInstance().getMainCamera()->getDesiredZoom() + 1.0f);
  } else if (InputManager::getInstance().isJustPressed("zoomOut")) {
    GameState::getInstance().getMainCamera()->zoomTo(
        GameState::getInstance().getMainCamera()->getDesiredZoom() - 1.0f);
  }
}

} // namespace script