#include "end.hpp"

#include "ColorPalette.hpp"
#include "LevelManager.hpp"
#include "DialogueManager.hpp"
#include "GameState.hpp"
#include "pressStart.hpp"
#include "sceneHelperFunctions.hpp"
#include "ScriptRunner.hpp"
#include "SceneManager.hpp"
#include <cstddef>

#include "Helpers.hpp"

const std::filesystem::path ROOT = Helper::getExecutableDir();

namespace SceneBuilder{

    void end() {

        GameState& gameState = GameState::getInstance();
        setupCameras(gameState);

        auto mainCam = gameState.getMainCamera();
        sf::RenderWindow& window = *gameState.getMainWindow();
        mainCam->goTo({0,0});

        // Particles Texture Setup
        setupParticles(window, ParticleManager::getInstance(), mainCam);

        // Text Font Setup
        DialogueManager& dialogueManager = DialogueManager::getInstance();
        setupTextAndDialogue(window, dialogueManager, mainCam);
        dialogueManager.loadDialoguesFromFile((ROOT / "assets\\dialogues\\dialogues.txt").string());
        dialogueManager.printByKey("thanks");

        LevelManager& levelManager = LevelManager::getInstance();

        ScriptRunner* scriptRunner = new ScriptRunner();
        scriptRunner->scripter.addScript(script::pressStart);

        levelManager.setBackgroundColor(ColorPalette::Black);

        sf::Texture dummy;

        //context. Imperative
        GeneralContext ctx = {
            {},
            dialogueManager.getAttachedTextParams(),
            nullptr,
            &dummy
        };
        SceneManager::getInstance().setContext(ctx);
    }
}