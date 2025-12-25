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
        dialogueManager.loadDialoguesFromFile("assets/dialogues/dialogues.txt");
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

        //title text
        // std::string markupMessage =
        //     "#position 0 0\n"
        //     "#boundary " + std::to_string(Constants::SCREEN_WIDTH/3) + "\n"
        //     "#alignment left\n" +
        //     "#effect typewriter 0.1\n" + 
        //     "So... That was it.\n" +
        //     "<color=yellow><anim=shake:2>for now!<anim=shake:2></color>\n" +
        //     "expect much more in the future :D\n" +
        //     "thanks to those who shared this <anim=sin>18</sin> day journey with me!\n" + 
        //     "-with love * - Danilo";

        // dialogueManager.print(markupMessage);
    }
}