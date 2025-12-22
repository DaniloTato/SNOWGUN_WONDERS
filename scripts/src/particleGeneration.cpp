#include "particleGeneration.hpp"
#include "ParticleManager.hpp"
#include "GameState.hpp"
#include "Constants.hpp"

#include "ScriptRunner.hpp"

namespace script{

    void particleGeneration(ScriptRunner& runner, const GeneralContext& ctx) {

        //Particles are not GameObjcts. Hence, they do not need queueing when trying to create them.

        const float snowSpawnRange =  5;
        ParticleManager::getInstance().emitSnow(
            GameState::getInstance().getMainCamera()->screenToWorld(
                {static_cast<float>(rand() % Constants::SCREEN_WIDTH * snowSpawnRange - Constants::SCREEN_WIDTH * snowSpawnRange * 0.5) ,-50},
                1.f
            )
        );
    }

}