#include "tutorialTriggers.hpp"
#include "BasicCollider.hpp"
#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "GeneralContext.hpp"
#include "LevelManager.hpp"
#include "SFML/System/Vector2.hpp"
#include "ScriptRunner.hpp"
#include "Helpers.hpp"
#include <array>

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
            Helper::TriggerOnce spawnRunningEnemies;
            Helper::TriggerOnce firstEnemySpawn;
            Helper::TriggerOnce hiddenRunningEnemies;
            Helper::TriggerOnce firstMissile;
            bool firstMissileSpawnOn = false;
            float firstMissileSpawnCounter;
            SecretLayerOppacity secretLayerOppacity;
        };
    }

    void tutorialTriggers(ScriptRunner& runner, const GeneralContext& ctx){
        auto& state = runner.scripter.getState<TutorialTriggersState>("tutorialTriggers");

        state.spawnRunningEnemies.check(
            (BasicCollider::isCollidingRect(
                ctx.player -> collider.getCollisionRect(ctx.player -> position),
                sf::Rect<float>(1922.f, 1532.f, 128.f, 68.f)
            )), 
            []{
                std::array<sf::Vector2f,6> points = {
                    sf::Vector2f{1679,1591},
                    sf::Vector2f{1663,1591},
                    sf::Vector2f{1645,1591},
                    sf::Vector2f{1620,1591},
                    sf::Vector2f{1600,1591},
                    sf::Vector2f{1550,1591},
                };

                for(auto& point : points){
                    EnemyManager::getInstance().queueCreateEnemy("runningToy", point);
                }
            }
        );

        state.firstEnemySpawn.check(
            (ctx.player->position.x > 2065.f && ctx.player->position.y < 1502.f),
            []{
                EnemyManager::getInstance().queueCreateEnemy("toy", {138*16,93*16});
            }
        );

        state.hiddenRunningEnemies.check(
            (ctx.player->position.x > 2226.f && ctx.player->position.y < 1502.f),
            []{
                std::array<sf::Vector2f,5> points = {
                    sf::Vector2f{139*16,92*16},
                    sf::Vector2f{138*16,93*16},
                    sf::Vector2f{135*16,92*16},
                    sf::Vector2f{134*16,93*16},
                    sf::Vector2f{131*16,92*16},
                };

                for(auto& point : points){
                    EnemyManager::getInstance().queueCreateEnemy("runningToy", point);
                }
            }
        );

        state.firstMissile.check(
            (ctx.player->position.x > 2300.f),
            [&state]{
                state.firstMissileSpawnOn = 90;
                state.firstMissileSpawnCounter = 1.f;
                EnemyManager::getInstance().queueCreateEnemy("missile", {155*16,81*16});
            }
        );

        state.firstMissileSpawnCounter -= GameState::getInstance().dt();
        if(state.firstMissileSpawnOn && state.firstMissileSpawnCounter <= 0.f){
            EnemyManager::getInstance().queueCreateEnemy("missile", {170*16,81*16});
            state.firstMissileSpawnCounter = 4.f;
        }

        //secret layer oppacity change
        state.secretLayerOppacity.update();
        if (BasicCollider::isCollidingRect(
                ctx.player -> collider.getCollisionRect(ctx.player -> position),
                sf::Rect<float>(133 * 16, 96 * 16, 7 * 16, 5 * 16)
            ))
        {
            state.secretLayerOppacity.targetOppacity = 0;
        } else{
            state.secretLayerOppacity.targetOppacity = 255;
        }

        
    }
}