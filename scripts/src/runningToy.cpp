#include "runningToy.hpp"
#include "TangibleObject.hpp"
#include "Helpers.hpp"

namespace script {

    namespace{

        struct runningToyState{
            Helper::TriggerOnce trigger1;
            Helper::TriggerOnce trigger2;
            Helper::TriggerOnce trigger3;
            Helper::TriggerOnce trigger4;
        };
    }

    void runningToy(TangibleObject& tangible, const GeneralContext& ctx){

        auto& state= tangible.scripter.getState<runningToyState>("runningToy");

        tangible.direction = 1;
        tangible.renderizer.setLayer(1.1);

        tangible.physics.setSpdx(3, PhysicsComponent::SpeedType::MOVEMENT);
        tangible.physics.updateX(tangible.position);

        tangible.collider.horizontalLevelCollision(tangible.position);

        state.trigger4.check((tangible.position.x >= 1710.f), [&tangible] {
            tangible.physics.setSpdy(-5, PhysicsComponent::SpeedType::MOVEMENT);
        });

        state.trigger1.check((tangible.position.x >= 1782.f), [&tangible] {
            tangible.physics.setSpdy(-5, PhysicsComponent::SpeedType::MOVEMENT);
        });

        state.trigger2.check((tangible.position.x >= 1864.f), [&tangible] {
            tangible.physics.setSpdy(-5, PhysicsComponent::SpeedType::MOVEMENT);
        });

        state.trigger3.check((tangible.position.x >= 1934.f), [&tangible] {
            tangible.physics.setSpdy(-5, PhysicsComponent::SpeedType::MOVEMENT);
        });

        tangible.physics.updateY(tangible.position);        
        if (tangible.collider.verticalLevelCollision(tangible.position)) {
            tangible.physics.setSpdy(0.f, PhysicsComponent::SpeedType::MOVEMENT);
        }
        
    }

}