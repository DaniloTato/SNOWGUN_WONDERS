
#include "GameCamera.hpp"
#include "Constants.hpp"
#include "InputManager.hpp"
#include "GameState.hpp"

namespace script{

    void followPlayer(GameCamera& camera, const CameraContext& ctx) {
        sf::Vector2f playerPosition = ctx.position;

        float zoom = camera.getZoom();
        sf::Vector2f halfViewSize(Constants::SCREEN_WIDTH / 2.f / zoom, Constants::SCREEN_HEIGHT / 2.f / zoom);
        sf::Vector2f desiredCamPos = playerPosition - halfViewSize;

        camera.goTo(desiredCamPos);

        if(InputManager::getInstance().isJustPressed("zoomIn")){
            GameState::getInstance().getMainCamera()->zoomTo(GameState::getInstance().getMainCamera()->getDesiredZoom() + 1.0f);
        } else if (InputManager::getInstance().isJustPressed("zoomOut")){
            GameState::getInstance().getMainCamera()->zoomTo(GameState::getInstance().getMainCamera()->getDesiredZoom() - 1.0f);
        }
    }

}