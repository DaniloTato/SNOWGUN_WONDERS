#pragma once
#include "TangibleObject.hpp"
#include "GeneralContext.hpp"

namespace script {
    void damageable(
        TangibleObject& tangible,
        const GeneralContext& ctx,
        int maxLife,
        float hitDuration,
        float deadDuration
    );

    namespace DamageFunctions{
        bool isDying(TangibleObject& tangible);
        bool isBeingHurt(TangibleObject& tangible);
        void kill(TangibleObject &tangible, float deadDuration);
    }
}