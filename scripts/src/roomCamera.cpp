#include "roomCamera.hpp"
#include "InputManager.hpp"
#include "GameState.hpp"
#include "Constants.hpp"
#include "LevelManager.hpp"

namespace script {

    void roomCamera(GameCamera& camera, const GeneralContext& ctx){

        const float FIXED_Y = 1460;

        sf::Vector2f playerPosition = *ctx.playerPosition;

        float zoom = camera.getZoom();
        float halfViewSizeX = Constants::SCREEN_WIDTH / 2.f / zoom;

        //maybe move getCameraPlayerRelation() to gameState?
        float desiredCamPosx = playerPosition.x - (halfViewSizeX - LevelManager::getInstance().getCameraPlayerRelation().x);

        camera.goTo({desiredCamPosx, FIXED_Y});

        if(InputManager::getInstance().isJustPressed("zoomIn")){
            GameState::getInstance().getMainCamera()->zoomTo(GameState::getInstance().getMainCamera()->getDesiredZoom() + 1.0f);
        } else if (InputManager::getInstance().isJustPressed("zoomOut")){
            GameState::getInstance().getMainCamera()->zoomTo(GameState::getInstance().getMainCamera()->getDesiredZoom() - 1.0f);
        }
    }

}