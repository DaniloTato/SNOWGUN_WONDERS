#include "healthPickup.hpp"
#include "CollectableManager.hpp"

namespace script {

    namespace{
        struct HealthPickupState {
            int heal = 1;
            bool collected = false;
        };
    }

    void healthPickup(TangibleObject& obj, const GeneralContext& ctx) {
        auto& state = obj.scripter.getState<HealthPickupState>("health");

        if (state.collected) return;

        if (BasicCollider::objectsColliding(ctx.player, &obj)) {
            state.collected = true;

            //ctx.player->heal(state.heal);
            CollectableManager::getInstance().queueDeleteCollectable(&obj);
        }
    }

}