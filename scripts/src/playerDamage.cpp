#include "playerDamage.hpp"
#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "BulletManager.hpp"
#include "PhysicsComponent.hpp"
#include "TangibleObject.hpp"
#include <SFML/Graphics.hpp>

namespace script {

    namespace {

        struct PlayerDamageState {
            bool invincible = false;
            float invincibleTimer = 0.f;

            bool knocked = false;
            float hitLockTimer = 0.f;
        };

        PlayerDamageState* getState(TangibleObject& tangible) {
            auto it = tangible.scripter.scriptState.find("playerDamage");
            if (it == tangible.scripter.scriptState.end())
                return nullptr;

            return &std::any_cast<PlayerDamageState&>(it->second);
        }
    }

    namespace PlayerDamageFunctions {

        bool isInvincible(TangibleObject& tangible) {
            auto* state = getState(tangible);
            return state ? state->invincible : false;
        }

        bool isKnocked(TangibleObject& tangible) {
            auto* state = getState(tangible);
            return state ? state->hitLockTimer > 0.f : false;
        }

        void QuitKnockedState(TangibleObject& tangible){
            auto* state = getState(tangible);
            state->knocked = false;
        }

        void forceHit(
            TangibleObject& tangible,
            int damage,
            float invincibilityTime,
            float hitLockTime,
            sf::Vector2f knockSpeed
        ) {
            auto* state = getState(tangible);
            if (!state || state->invincible)
                return;

            GameState::getInstance().changePlayerHealth(-damage);

            tangible.physics.setSpdx(
                knockSpeed.x,
                PhysicsComponent::SpeedType::MOVEMENT
            );

            tangible.physics.setSpdy(
                knockSpeed.y,
                PhysicsComponent::SpeedType::MOVEMENT
            );

            state->invincible = true;
            state->invincibleTimer = invincibilityTime;
            state->knocked = true;
            state->hitLockTimer = hitLockTime;
        }
    }

    void playerDamage(
        TangibleObject& tangible,
        const GeneralContext& ctx,
        float invincibilityDuration,
        sf::Vector2f knockSpeed
    ) {
        auto& state =
            tangible.scripter.getState<PlayerDamageState>("playerDamage");

        float dt = GameState::getInstance().dt();

        if (state.invincible) {
            state.invincibleTimer -= dt;
            if (state.invincibleTimer <= 0.f) {
                state.invincible = false;
                tangible.renderizer.showSprite();
            } else {
                tangible.renderizer.toggleShowEvery(0.08f);
            }
        }

        if (state.hitLockTimer > 0.f)
            state.hitLockTimer -= dt;

        if (state.invincible)
            return;

        /*Check if player is colliding with bullet*/
        Bullet* bulletHit = BulletManager::getInstance()
            .isCollidingWithBullet(tangible, BulletManager::I_AM_PLAYER);

        if (bulletHit) {
            bulletHit->die();

            PlayerDamageFunctions::forceHit(
                tangible,
                1,
                invincibilityDuration,
                0.5f,
                {knockSpeed.x * -tangible.direction, knockSpeed.y}
            );
            tangible.playSound("hit");
        }
        /*Check if player is colliding with bullet*/

        /*Check if player is colliding with enemy*/
        TangibleObject* enemyHit = EnemyManager::getInstance()
            .isCollidingWithEnemy(tangible);

        if(enemyHit && !bulletHit){
            PlayerDamageFunctions::forceHit(
                tangible,
                1,
                invincibilityDuration,
                0.5f,
                {knockSpeed.x * -tangible.direction, knockSpeed.y}
            );
            tangible.playSound(GameState::getInstance().getPlayerHealth() > 0 ? "hit" : "die", 200);
        }
        /*Check if player is colliding with enemy*/
    }
}