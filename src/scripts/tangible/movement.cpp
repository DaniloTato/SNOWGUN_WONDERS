#include "TangibleObject.hpp"
#include "InputManager.hpp"

namespace script{

    void movement(TangibleObject& tangible, const GeneralContext& ctx) {

        if(InputManager::getInstance().isPressed("left")){
            tangible.position.x -= 1.f;
        } else if (InputManager::getInstance().isPressed("right")){
            tangible.position.x += 1.f;
        }
        tangible.collider.horizontalLevelCollision(tangible.position);
        if(InputManager::getInstance().isPressed("up")){
            tangible.position.y -= 1.f;
        } else if (InputManager::getInstance().isPressed("down")){
            tangible.position.y += 1.f;
        }
        tangible.collider.verticalLevelCollision(tangible.position);
    }

}