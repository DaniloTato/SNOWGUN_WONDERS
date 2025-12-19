#include "damageable.hpp"
#include "BulletManager.hpp"
#include "ParticleManager.hpp"
#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "PhysicsComponent.hpp"
#include <limits>

namespace script {

    namespace {
        const float EXPLOSION_COOLDOWN = 0.1f;

        struct DamageState {
            int life;
            float hitTimer = 0.f;
            float deadTimer = std::numeric_limits<float>::max();
            float explosionCooldown = EXPLOSION_COOLDOWN;

            DamageState(int maxLife)
            : life(maxLife) {}
        };

        bool isDead(const DamageState& s) {
            return s.deadTimer != std::numeric_limits<float>::max();
        }
    }

    namespace DamageFunctions{
        bool isDying(const TangibleObject& tangible) {
            auto it = tangible.scripter.scriptState.find("damageable");
            if (it == tangible.scripter.scriptState.end()) return false;

            const auto& state = std::any_cast<const DamageState&>(it->second);
            return state.deadTimer != std::numeric_limits<float>::max();
        }

        bool isBeingHurt(const TangibleObject &tangible){
            auto it = tangible.scripter.scriptState.find("damageable");
            if (it == tangible.scripter.scriptState.end()) return false;

            const auto& state = std::any_cast<const DamageState&>(it->second);
            return state.hitTimer > 0.f;
        }
    }

    void damageable(
        TangibleObject& tangible,
        const GeneralContext& ctx,
        int maxLife,
        float hitDuration,
        float deadDuration
    ) {
        auto& state = tangible.scripter.getState<DamageState>("damageable", maxLife);

        float dt = GameState::getInstance().dt();
        state.hitTimer -= dt;
        state.explosionCooldown -= dt;

        if (isDead(state)) {
            state.deadTimer -= dt;

            if (state.deadTimer <= 0.f) {
                EnemyManager::getInstance().queueDeleteEnemy(&tangible);
                return;
            }

            if (state.explosionCooldown <= 0.f) {
                ParticleManager::getInstance().emitExplosion(
                    tangible.position + sf::Vector2f(7,4), 3
                );
                state.explosionCooldown = EXPLOSION_COOLDOWN;
            }

            tangible.offset = {
                static_cast<float>(rand() % 4 - 2), 0
            };
            tangible.renderizer.toggleShowEvery(0.05f);
            return;
        }

        if (state.hitTimer > 0.f) {
            tangible.offset = {
                static_cast<float>(rand() % 4 - 2), 0
            };
        } else {
            tangible.offset = {0,0};
        }

        /*Check for bullet collision*/
        Bullet* hit = BulletManager::getInstance()
            .isCollidingWithBullet(tangible, BulletManager::I_AM_NOT_PLAYER);

        if (hit) {
            hit->die();
            state.life--;
            state.hitTimer = hitDuration;

            if (state.life <= 0 && !isDead(state)) {
                state.deadTimer = deadDuration;
            }
        }
        /*Check for bullet collision*/

        /*Check for player kick hitbox*/
        if (ctx.player && ctx.player->attackHitbox.has_value()) {

            const auto& hitbox = ctx.player->attackHitbox.value();

            sf::FloatRect attackRect =
                hitbox.collider.getCollisionRect(ctx.player->position);

            sf::FloatRect enemyRect =
                tangible.collider.getCollisionRect(tangible.position);

            if (BasicCollider::isCollidingRect(attackRect, enemyRect)) {

                ParticleManager::getInstance().emitStars(tangible.position + sf::Vector2f(4.f,4.f),8);

                state.life -= ctx.player->attackHitbox->damage;
                state.hitTimer = hitDuration;

                tangible.physics.setSpdx(
                    5.f * ctx.player->direction,
                    PhysicsComponent::SpeedType::MOVEMENT
                );

                tangible.physics.setSpdy(
                    -3.f,
                    PhysicsComponent::SpeedType::MOVEMENT
                );

                const int playerKickSpeed = ctx.player->physics.getSpdx(PhysicsComponent::SpeedType::KICK);
                const int playerMovementSpeed = ctx.player->physics.getSpdx(PhysicsComponent::SpeedType::MOVEMENT);

                ctx.player->physics.setSpdx(
                    (playerKickSpeed + playerMovementSpeed) * -1, 
                    PhysicsComponent::SpeedType::KICK);

                if (state.life <= 0 && !isDead(state)) {
                    state.deadTimer = deadDuration;
                }

                ctx.player->attackHitbox.reset();

                return;
            }
        }
        /*Check for player kick hitbox*/
    }
}