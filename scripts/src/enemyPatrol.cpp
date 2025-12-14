#include "enemyPatrol.hpp"
#include "Bullet.hpp"
#include "BulletManager.hpp"
#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "ParticleManager.hpp"
#include "SFML/System/Vector2.hpp"
#include <limits>

namespace script {

    namespace {
        const float EXPLOTION_COOLDOWN = 0.1;
        const float DEAD_STATE_DURATION = 0.65;
        const float HIT_STATE_DURATION = 0.8;

        struct EnemyPatrolState {
            int life = 10;
            float hitState = 0;
            float deadState = std::numeric_limits<float>::max();
            float explosionCooldown = EXPLOTION_COOLDOWN;
        };

        bool isDeathStateActive(EnemyPatrolState& state){
            return state.deadState != std::numeric_limits<float>::max();
        }
    }

    void enemyPatrol(TangibleObject& tangible, const GeneralContext& ctx){

        auto& stateAny = tangible.scripter.scriptState["enemyPatrol"];
        if (!stateAny.has_value()) {
            stateAny = EnemyPatrolState{};
        }

        auto& state = std::any_cast<EnemyPatrolState&>(stateAny);

        state.hitState -= GameState::getInstance().dt();
        state.explosionCooldown -= GameState::getInstance().dt();

        if(isDeathStateActive(state)){
            state.deadState -= GameState::getInstance().dt();
            if(state.deadState <= 0){
                EnemyManager::getInstance().queueDeleteEnemy(&tangible);
            }

            if(state.explosionCooldown <= 0){
                ParticleManager::getInstance().emitExplosion(
                tangible.position + sf::Vector2f(7,4),
                3
                );
                state.explosionCooldown = EXPLOTION_COOLDOWN;
            }

            tangible.offset = {static_cast<float>(rand() % 4 - 2), 0};

            tangible.renderizer.toggleShowEvery(0.05);
        } else{
            if(state.hitState <= 0){
                tangible.offset = {0,0};
                tangible.physics.setSpdx(1 * tangible.direction);
            }else{
                tangible.offset = {static_cast<float>(rand() % 4 - 2), 0};
            }
        }

        tangible.physics.updateX(tangible.position);
        if(tangible.collider.horizontalLevelCollision(tangible.position)){
            tangible.direction *= -1;
        }

        tangible.physics.updateY(tangible.position);
        if(tangible.collider.verticalLevelCollision(tangible.position)){
            tangible.physics.setSpdy(0.f);
        }

        Bullet* hit = BulletManager::getInstance().isCollidingWithBullet(tangible);
        if (hit) {
            hit->die();
            state.life -= 1;

            state.hitState = HIT_STATE_DURATION;

            if(state.life <= 0 && !isDeathStateActive(state)){
                state.deadState = DEAD_STATE_DURATION;
            }
        }
    }
}