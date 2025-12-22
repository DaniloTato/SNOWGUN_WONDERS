#include "tutorial.hpp"

#include "CollectableManager.hpp"
#include "ColorPalette.hpp"
#include "LevelManager.hpp"
#include "DialogueManager.hpp"
#include "ParticleManager.hpp"
#include "GameState.hpp"
#include "followPlayer.hpp"
#include "sceneHelperFunctions.hpp"
#include "ScriptRunner.hpp"
#include "SceneManager.hpp"

#include "levelCreatorInputs.hpp"
#include "particleGeneration.hpp"
#include "particleGeneration.hpp"
#include "tutorialTriggers.hpp"

namespace SceneBuilder{

    void tutorial() {
        LevelManager& levelManager = LevelManager::getInstance();
        DialogueManager& dialogueManager = DialogueManager::getInstance();
        ParticleManager& particleManager = ParticleManager::getInstance();
        GameState& gameState = GameState::getInstance();
        sf::RenderWindow& window = *gameState.getMainWindow();

        setupCameras(gameState);
        auto mainCam = gameState.getMainCamera();
        mainCam->scripter.addScript(script::followPlayer);

        ScriptRunner* scriptRunner = new ScriptRunner();
        scriptRunner->scripter.addScript(script::levelCreatorInputs);
        scriptRunner->scripter.addScript(script::particleGeneration);
        scriptRunner->scripter.addScript(script::tutorialTriggers);

        // Player setup
        TangibleObject* player = createPlayer(window, loadTexture("assets/snowman_animation.png"), mainCam, {16.f * 100, 16.f * 98.f});

        // Bullets
        static sf::Texture bulletTexture;
        bulletTexture.loadFromFile("assets/bullet.png");

        // Particles Texture Setup
        setupParticles(window, particleManager, mainCam);

        // Text Font Setup
        setupTextAndDialogue(window, dialogueManager, mainCam);
        dialogueManager.loadDialoguesFromFile("assets/dialogues/dialogues.txt");

        levelManager.loadLevel(window, GameState::getInstance().getMainCamera(), "assets/level_data/tutorial.json");

        levelManager.setBackgroundColor(ColorPalette::ElectricBlue);

        CollectableManager::getInstance().queueCreateCollectable("chest", {124 *16, 99*16});
        CollectableManager::getInstance().queueCreateCollectable("chest", {138 *16, 98*16});

        //context. Imperative
        GeneralContext ctx = {
            &(player -> position),
            dialogueManager.getAttachedTextParams(),
            player,
            &bulletTexture
        };
        SceneManager::getInstance().setContext(ctx);
    }
}