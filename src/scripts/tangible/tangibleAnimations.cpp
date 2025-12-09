#include "TangibleObject.hpp"
#include "InputManager.hpp"

namespace script{

    void tangibleAnimations(TangibleObject& tangible, const GeneralContext& ctx) {

        if(InputManager::getInstance().isPressed("left") || InputManager::getInstance().isPressed("right")){
            tangible.animator.setState("walk");
        } else {
            tangible.animator.setState("idle");
        }
    }

}