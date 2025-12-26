#include "titleScreen.hpp"

#include "ColorPalette.hpp"
#include "LevelManager.hpp"
#include "DialogueManager.hpp"
#include "GameState.hpp"
#include "pressStart.hpp"
#include "sceneHelperFunctions.hpp"
#include "ScriptRunner.hpp"
#include "SceneManager.hpp"
#include "Helpers.hpp"
#include "Constants.hpp"
#include <cstddef>

const std::filesystem::path ROOT = Helper::getExecutableDir();

namespace SceneBuilder{

    void titleScreen() {

        GameState& gameState = GameState::getInstance();
        setupCameras(gameState);

        auto mainCam = gameState.getMainCamera();
        sf::RenderWindow& window = *gameState.getMainWindow();

        // Particles Texture Setup
        setupParticles(window, ParticleManager::getInstance(), mainCam);

        // Text Font Setup
        DialogueManager& dialogueManager = DialogueManager::getInstance();
        setupTextAndDialogue(window, dialogueManager, mainCam);
        dialogueManager.loadDialoguesFromFile(ROOT / "assets/dialogues/dialogues.txt");

        LevelManager& levelManager = LevelManager::getInstance();

        ScriptRunner* scriptRunner = new ScriptRunner();
        scriptRunner->scripter.addScript(script::pressStart);

        levelManager.setBackgroundColor(ColorPalette::ElectricBlue);

        sf::Texture dummy;

        //context. Imperative
        GeneralContext ctx = {
            {},
            dialogueManager.getAttachedTextParams(),
            nullptr,
            &dummy
        };
        SceneManager::getInstance().setContext(ctx);

        sf::Texture& fontTexture = Helper::loadTexture(ROOT / "assets/font.png");

        //title text
        RenderizerParameters params{
            window,
            fontTexture,
            sf::IntRect(),
            {0.f, 0.f},
            mainCam,
            Constants::TEXT_LAYER,
            1.f
        };
        GameText* title = new GameText(params);
        title->setFontAtlas(fontTexture, 9, 8, 95, 32);
        std::string markupLife =
            "#position 0 90\n"
            "#boundary " + std::to_string(Constants::SCREEN_WIDTH/3) + "\n"
            "#alignment center\n" +
            "SNOWGUN\n<color=purple><anim=sin>WONDERS</anim></color>\n";

        title -> loadFromMarkup(markupLife);

        GameText* enter = new GameText(params);
        enter->setFontAtlas(fontTexture, 9, 8, 95, 32);
        std::string markupEnter =
            "#position 0 150\n"
            "#boundary " + std::to_string(Constants::SCREEN_WIDTH/3) + "\n"
            "#alignment center\n" +
            "#effect typewriter 0.2\n" +
            "<color=yellow><anim=shake:1>-PRESS ENTER TO START-</anim><color=purple>";

        enter -> loadFromMarkup(markupEnter);

        GameText* version = new GameText(params);
        version->setFontAtlas(fontTexture, 9, 8, 95, 32);
        std::string markupVersion =
            "#position 0 30\n"
            "#boundary " + std::to_string(Constants::SCREEN_WIDTH/3) + "\n"
            "#alignment center\n" +
            "v0.1.0-alpha";

        version -> loadFromMarkup(markupVersion);

        LevelManager::getInstance().setBackgroundColor(ColorPalette::Black);
    }
}