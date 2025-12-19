#include "movement.hpp"

#include "BasicCollider.hpp"
#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "PhysicsComponent.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include "BulletManager.hpp"

namespace script{

    namespace{
        const float MAX_MOVEMENT_LOCK = 0.1;

        const float KICK_JUMP_HEIGHT = -3.f;
        const float KICK_JUMP_X_SPEED = 4.f;
        const float KICK_DURATION = 0.18f;
        const int KICK_DAMAGE = 4;

        const int SPRITE_DIRECTION_OFFSET = 8.f;

        struct MovementState {
            float movementLock = 0;
            int timeInAir = 0;

            bool kicking = false;
            float kickTimer = 0.f;
        };
    }

    void movement(TangibleObject& tangible, const GeneralContext& ctx) {

        auto& stateAny = tangible.scripter.scriptState["movement"];
        if (!stateAny.has_value()) {
            stateAny = MovementState{};
        }
        auto& state = std::any_cast<MovementState&>(stateAny);

        std::string desiredAnimation = "idle";

        int lookingUp = 0;
        if (InputManager::getInstance().isPressed("up")){
            lookingUp = 1;
        } else if(InputManager::getInstance().isPressed("down")){
            lookingUp = -1;
        }

        if(InputManager::getInstance().isPressed("left")){
            tangible.direction = -1;

            if(state.movementLock <= 0){
                tangible.physics.setSpdx(-3.f, PhysicsComponent::SpeedType::MOVEMENT);
            }else{
                tangible.physics.setSpdx(-1.f, PhysicsComponent::SpeedType::MOVEMENT);
            }
            desiredAnimation = "walk";
        } else if (InputManager::getInstance().isPressed("right")){
            tangible.direction = 1;

            if(state.movementLock <= 0){
                tangible.physics.setSpdx(3.f, PhysicsComponent::SpeedType::MOVEMENT);
            }else{
                tangible.physics.setSpdx(1.f, PhysicsComponent::SpeedType::MOVEMENT);
            }
            desiredAnimation = "walk";
        }

        tangible.physics.updateX(tangible.position);
        tangible.collider.horizontalLevelCollision(tangible.position);

        if(InputManager::getInstance().isJustPressed("jump")){
            tangible.physics.setSpdy(-5.f, PhysicsComponent::SpeedType::MOVEMENT);
        }

        tangible.physics.updateY(tangible.position);
        if(tangible.collider.verticalLevelCollision(tangible.position)){
            tangible.physics.setSpdy(0.f, PhysicsComponent::SpeedType::MOVEMENT);
            state.timeInAir = 0;
        }else{
            state.timeInAir++;
        }

        if(state.timeInAir > 0){
            desiredAnimation = "jump_once";
        }

        /*Kick logic*/
        if (InputManager::getInstance().isJustPressed("kick")
            && !state.kicking) {

            state.kicking = true;
            state.kickTimer = KICK_DURATION;

            tangible.physics.setSpdy(KICK_JUMP_HEIGHT, PhysicsComponent::SpeedType::MOVEMENT);
            tangible.physics.setSpdx(KICK_JUMP_X_SPEED * tangible.direction,PhysicsComponent::SpeedType::KICK);

            /*Crete Kick Hitbox*/
            BasicCollider kickCollider;
            kickCollider.setSize({8.f, 20.f});
            kickCollider.setOffset({
                tangible.direction == 1 ? 12.f : -2,
                0.f
            });
            AttackHitbox kickHitbox = {kickCollider, true,KICK_DAMAGE, KICK_DURATION};
            tangible.attackHitbox = kickHitbox;
            /*Crete Kick Hitbox*/
        }

        if (state.kicking) {
            state.kickTimer -= GameState::getInstance().dt();

            desiredAnimation = "kick_once";

            if (state.kickTimer <= 0.f && state.timeInAir <= 0) {
                state.kicking = false;
            }
        } else{
            tangible.attackHitbox.reset();
        }
        /*Kick logic*/

        RenderizerParameters bulletParams{
            *GameState::getInstance().getMainWindow(),
            *ctx.bulletTexture,
            {0,0,15,15},
            tangible.position,
            GameState::getInstance().getMainCamera(),
            0.f,
            1.f
        };

        state.movementLock -= GameState::getInstance().dt();

        if(InputManager::getInstance().isJustPressed("shoot") && !state.kicking){
            state.movementLock = MAX_MOVEMENT_LOCK;

            sf::Vector2f bulletSpeed;
            if (lookingUp == 1){
                bulletSpeed = {0.f,-4.f};
            } else if (lookingUp == -1){
                bulletSpeed = {0.f,4.f};
            }else{
                bulletSpeed = {4.f * tangible.direction,0.f};
            }

            BulletManager::getInstance().queueSpawn(
                bulletParams,
                Bullet::Type::Normal,
                bulletSpeed,
                sf::Vector2f(0.f,0.f),
                8.f,
                300.f,
                BulletManager::I_AM_PLAYER
            );
        }

        tangible.animator.play(desiredAnimation);

        //offset due to sprite not being centered.
        if(tangible.direction == -1){
            tangible.offset = {-SPRITE_DIRECTION_OFFSET, 0.f};
        } else{
            tangible.offset = {0.f, 0.f};
        }
    }

}