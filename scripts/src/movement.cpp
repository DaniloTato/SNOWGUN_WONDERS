#include "movement.hpp"

#include "BasicCollider.hpp"
#include "SceneManager.hpp"
#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "PhysicsComponent.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include "BulletManager.hpp"
#include "playerDamage.hpp"

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
            bool godMode = false;
            float kickTimer = 0.f;
        };
    }

    void movement(TangibleObject& tangible, const GeneralContext& ctx) {

        playerDamage(
            tangible,
            ctx,
            1.f,
            {5,-3}
        );

        if(InputManager::getInstance().isJustPressed("changeGun")){
            tangible.playSound("doKick");
        }

        if (PlayerDamageFunctions::isKnocked(tangible) || GameState::getInstance().getPlayerHealth() <= 0) {

            tangible.physics.updateX(tangible.position);
            tangible.collider.horizontalLevelCollision(tangible.position);

            tangible.physics.updateY(tangible.position);

            if (tangible.collider.verticalLevelCollision(tangible.position)) {
                tangible.physics.setSpdy(0.f, PhysicsComponent::SpeedType::MOVEMENT);
                if(GameState::getInstance().getPlayerHealth() > 0){
                    PlayerDamageFunctions::QuitKnockedState(tangible);
                } else{
                    SceneManager::getInstance().reloadCurrentScene();
                }
            }

            tangible.animator.play("damaged_once");
            return;
        }

        auto& state = tangible.scripter.getState<MovementState>("movement");

        std::string desiredAnimation = "idle";

        int lookingUp = 0;
        if (InputManager::getInstance().isPressed("up")){
            lookingUp = 1;
        } else if(InputManager::getInstance().isPressed("down")){
            lookingUp = -1;
        }

        if(state.godMode){
            tangible.physics.turnOnYFriction();
            tangible.physics.gravity = 0.f;

            if(InputManager::getInstance().isPressed("left")){
                tangible.physics.setSpdx(-3.f, PhysicsComponent::SpeedType::MOVEMENT);
            } else if(InputManager::getInstance().isPressed("right")){
                tangible.physics.setSpdx(3.f, PhysicsComponent::SpeedType::MOVEMENT);
            }

            tangible.physics.updateX(tangible.position);
            
            if(InputManager::getInstance().isPressed("up")){
                tangible.physics.setSpdy(-3.f, PhysicsComponent::SpeedType::MOVEMENT);
            } else if(InputManager::getInstance().isPressed("down")){
                tangible.physics.setSpdy(3.f, PhysicsComponent::SpeedType::MOVEMENT);
            }

            tangible.physics.updateY(tangible.position);

        } else{
            tangible.physics.turnOffYFriction();
            tangible.physics.gravity = 0.3f;

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

            if(InputManager::getInstance().isJustPressed("jump") && state.timeInAir == 0){
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

                tangible.playSound("doKick");

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

        }

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
            tangible.playSound("shoot");

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
                GameState::getInstance().getWeaponSelection(),
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

        if(InputManager::getInstance().isJustPressed("godMode")){
            state.godMode = !state.godMode;
        }
    }

}