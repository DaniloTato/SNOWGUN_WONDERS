#include "movement.hpp"

#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "PhysicsComponent.hpp"

namespace script{

    void movement(TangibleObject& tangible, const GeneralContext& ctx) {

        if(InputManager::getInstance().isPressed("left")){
            tangible.direction = -1;
            tangible.physics.setSpdx(-3.f);
        } else if (InputManager::getInstance().isPressed("right")){
            tangible.direction = 1;
            tangible.physics.setSpdx(3.f);
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
    }

}