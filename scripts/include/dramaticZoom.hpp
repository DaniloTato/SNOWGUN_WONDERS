#pragma once

#include "GameCamera.hpp"
#include "GeneralContext.hpp"

namespace script {

    void dramaticZoom(GameCamera& camera, const GeneralContext& ctx);
    void startDramaticZoom(GameCamera& camera, float zoomInLevel, float pauseDuration);

}