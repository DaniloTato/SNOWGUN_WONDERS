#include "tutorialTriggers.hpp"
#include "BasicCollider.hpp"
#include "EnemyManager.hpp"
#include "GeneralContext.hpp"
#include "LevelManager.hpp"
#include "ScriptRunner.hpp"
#include "Helpers.hpp"

namespace script {

    float const HIDDEN_LAYER_LERP_SPEED = 0.05;

    namespace{

        struct SecretLayerOppacity{
            float currentOppacity = 255;
            float targetOppacity = 255;

            void update(){
                currentOppacity = Helper::lerp(currentOppacity, targetOppacity, HIDDEN_LAYER_LERP_SPEED);
                LevelManager::getInstance().setSecretLayerOppacity(currentOppacity);
            }
        };

        struct TutorialTriggersState{
            SecretLayerOppacity secretLayerOppacity;
        };
    }

    void tutorialTriggers(ScriptRunner& runner, const GeneralContext& ctx){
        auto& state = runner.scripter.getState<TutorialTriggersState>("tutorialTriggers");

        EnemyManager::getInstance().checkSpawnTriggers(ctx.player->position);

        //secret layer oppacity change
        state.secretLayerOppacity.update();
        if (BasicCollider::isCollidingRect(
                ctx.player -> collider.getCollisionRect(ctx.player -> position),
                sf::Rect<float>(133 * 16, 96 * 16, 7 * 16, 5 * 16)
            )
            ||
            BasicCollider::isCollidingRect(
                ctx.player -> collider.getCollisionRect(ctx.player -> position),
                Helper::makeRectFromPoints(6640, 1380, 6785, 1430)
            )
            ||
            BasicCollider::isCollidingRect(
                ctx.player -> collider.getCollisionRect(ctx.player -> position),
                Helper::makeRectFromPoints(6050, 1270, 6300, 1330)
            )
        )
        {
            state.secretLayerOppacity.targetOppacity = 0;
        } else{
            state.secretLayerOppacity.targetOppacity = 255;
        }

        
    }
}