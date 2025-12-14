#include "movement.hpp"

#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "PhysicsComponent.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include "BulletManager.hpp"

namespace script{

    namespace{
        const float MAX_MOVEMENT_LOCK = 0.1;

        struct MovementState {
            float movementLock = 0;
        };

        MovementState movementState;
    }

    void movement(TangibleObject& tangible, const GeneralContext& ctx) {

        if(InputManager::getInstance().isPressed("left")){
            tangible.direction = -1;

            if(movementState.movementLock <= 0){
                tangible.physics.setSpdx(-3.f);
            }else{
                tangible.physics.setSpdx(-1.f);
            }
        } else if (InputManager::getInstance().isPressed("right")){
            tangible.direction = 1;

            if(movementState.movementLock <= 0){
                tangible.physics.setSpdx(3.f);
            }else{
                tangible.physics.setSpdx(1.f);
            }
        }

        tangible.physics.updateX(tangible.position);
        tangible.collider.horizontalLevelCollision(tangible.position);

        if(InputManager::getInstance().isJustPressed("jump")){
            tangible.physics.setSpdy(-5.f);
        }

        tangible.physics.updateY(tangible.position);
        if(tangible.collider.verticalLevelCollision(tangible.position)){
            tangible.physics.setSpdy(0.f);
        }

        RenderizerParameters bulletParams{
            ctx.window,
            ctx.bulletTexture,
            {0,0,15,15},
            tangible.position,
            GameState::getInstance().getMainCamera(),
            0.f,
            1.f
        };

        movementState.movementLock -= GameState::getInstance().dt();

        if(InputManager::getInstance().isJustPressed("shoot")){
            movementState.movementLock = MAX_MOVEMENT_LOCK;
            BulletManager::getInstance().queueSpawn(
                bulletParams,
                BulletType::Normal,
                sf::Vector2f(4.f * tangible.direction,0.f),
                sf::Vector2f(0.f,0.f),
                8.f,
                800.f
            );
        }
    }

}