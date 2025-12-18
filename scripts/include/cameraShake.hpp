#pragma once

#include "GameCamera.hpp"
#include "GeneralContext.hpp"

namespace script {

    void cameraShake(GameCamera& camera, const GeneralContext& ctx);

    namespace ShakeFunctions{
        bool isShaking(GameCamera& camera);
    }

}