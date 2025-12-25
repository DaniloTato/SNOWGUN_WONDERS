
#include "followPlayer.hpp"

#include "GameCamera.hpp"
#include "Constants.hpp"
#include "InputManager.hpp"
#include "GameState.hpp"
#include "LevelManager.hpp"
#include "SceneManager.hpp"

namespace script{

    void followPlayer(GameCamera& camera, const GeneralContext& ctx) {
        sf::Vector2f playerPosition = *ctx.playerPosition;

        float zoom = camera.getZoom();
        sf::Vector2f halfViewSize(Constants::SCREEN_WIDTH / 2.f / zoom, Constants::SCREEN_HEIGHT / 2.f / zoom);
        sf::Vector2f desiredCamPos = playerPosition - (halfViewSize - LevelManager::getInstance().getCameraPlayerRelation());

        if(desiredCamPos.x > 3000 && desiredCamPos.y > 87*16){
            desiredCamPos.y = 87*16;
            if(playerPosition.y > desiredCamPos.y + Constants::SCREEN_HEIGHT/camera.getZoom()){
                SceneManager::getInstance().reloadCurrentScene();
            }
        }

        camera.goTo(desiredCamPos);

        if(InputManager::getInstance().isJustPressed("zoomIn")){
            GameState::getInstance().getMainCamera()->zoomTo(GameState::getInstance().getMainCamera()->getDesiredZoom() + 1.0f);
        } else if (InputManager::getInstance().isJustPressed("zoomOut")){
            GameState::getInstance().getMainCamera()->zoomTo(GameState::getInstance().getMainCamera()->getDesiredZoom() - 1.0f);
        }
    }

}