#include "missileAI.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "ParticleManager.hpp"
#include "cameraShake.hpp"
#include "damageable.hpp"

namespace script {

    namespace{

        const int MAX_LIFE = 10;
        const float SPEED = 1.f;
        const float EXPLOSION_SPEED = 0.1;
        const int EXPLOSION_DAMAGE = 1;
        const float DEAD_DURATION = 1.f;
        const float MAXIMUM_EXPLOSION_RADIUS = 40.f;

        const float CAMERA_SHAKE_DURATION = 1.f;
        const float CAMERA_SHAKE_INTENSITY = 20.f;

        struct MissileState{
            float currentExplosionRadius = 0;
            Helper::TriggerOnce cameraShake;
        };
    }

    void missileAI(TangibleObject& tangible, const GeneralContext& ctx){

        script::damageable(
            tangible, ctx,
            MAX_LIFE,
            0.6f,
            DEAD_DURATION
        );

        auto& status = tangible.scripter.getState<MissileState>("missileAI");

        tangible.offset = {static_cast<float>(rand() % 2 - 2), 0.f};

        bool isDying = script::DamageFunctions::isDying(tangible);

        status.cameraShake.check((isDying),
            []{
                script::ShakeFunctions::startShake(*GameState::getInstance().getMainCamera(), CAMERA_SHAKE_DURATION, CAMERA_SHAKE_INTENSITY);
            }
        );

        if(isDying){

            if(!tangible.isPlayingAnySound()){
                tangible.playSound("explosion", 50);
            }

            tangible.renderizer.setLayer(0.f); //change layer so it renders behind explosion
            status.currentExplosionRadius = Helper::lerp(status.currentExplosionRadius, MAXIMUM_EXPLOSION_RADIUS, EXPLOSION_SPEED);

            sf::Vector2f explosionCenter = tangible.position + sf::Vector2f(8.f, 24.f);
            ParticleManager::getInstance().emitMediumExplosion(explosionCenter, 2, status.currentExplosionRadius);

            BasicCollider explosionCollider;
            explosionCollider.setSize({ 2.f * status.currentExplosionRadius, 2.f * status.currentExplosionRadius });

            explosionCollider.setOffset({
                explosionCenter.x - status.currentExplosionRadius - tangible.position.x + 10,
                explosionCenter.y - status.currentExplosionRadius - tangible.position.y
            });

            AttackHitbox explosionHitbox = {
                explosionCollider,
                true,
                EXPLOSION_DAMAGE,
                10.f
            };

            tangible.attackHitbox = explosionHitbox;
            return;
        }

        ParticleManager::getInstance().emitSmoke(tangible.position + sf::Vector2f(23,15), 1);

        tangible.physics.setSpdx(SPEED * tangible.direction, PhysicsComponent::SpeedType::MOVEMENT);
        tangible.physics.updateX(tangible.position);
        if (tangible.collider.horizontalLevelCollision(tangible.position)) {
            script::DamageFunctions::kill(tangible, DEAD_DURATION);
        }

        tangible.physics.setSpdy(SPEED, PhysicsComponent::SpeedType::MOVEMENT);
        tangible.physics.updateY(tangible.position);
        if (tangible.collider.verticalLevelCollision(tangible.position)) {
            script::DamageFunctions::kill(tangible, DEAD_DURATION);
        }
    }
}