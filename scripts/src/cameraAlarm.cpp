#include "cameraAlarm.hpp"
#include "GameCamera.hpp"
#include "cameraShake.hpp"

namespace script{

    namespace{
        struct AlarmState{

        };
    }

    void cameraAlarm(GameCamera& camera, const GeneralContext& ctx) {
        cameraShake(camera, ctx);

        if (script::ShakeFunctions::isShaking(camera)){
            
        }
    }

}