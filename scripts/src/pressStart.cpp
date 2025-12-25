#include "pressStart.hpp"
#include "InputManager.hpp"
#include "SceneManager.hpp"

namespace script {

    void pressStart(ScriptRunner& tangible, const GeneralContext& ctx){
        if(InputManager::getInstance().isJustPressed("start")){
            SceneManager::getInstance().loadScene("tutorial");
        }
    }
}