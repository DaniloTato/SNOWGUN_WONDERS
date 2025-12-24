#include "setupMainLevelScene.hpp"

#include "LevelManager.hpp"
#include "DialogueManager.hpp"
#include "ParticleManager.hpp"
#include "GameState.hpp"
#include "sceneHelperFunctions.hpp"
#include "ScriptRunner.hpp"
#include "SceneManager.hpp"
#include "Helpers.hpp"

#include "levelCreatorInputs.hpp"
#include "roomCamera.hpp"

#include "Constants.hpp"

namespace SceneBuilder{

    void setupMainLevelScene() {
        LevelManager& levelManager = LevelManager::getInstance();
        DialogueManager& dialogueManager = DialogueManager::getInstance();
        ParticleManager& particleManager = ParticleManager::getInstance();
        GameState& gameState = GameState::getInstance();
        sf::RenderWindow& window = *gameState.getMainWindow();

        setupCameras(gameState);
        auto mainCam = gameState.getMainCamera();
        mainCam->scripter.addScript(script::roomCamera);

        ScriptRunner* scriptRunner = new ScriptRunner();
        scriptRunner->scripter.addScript(script::levelCreatorInputs);

        // Player setup
        TangibleObject* player = createPlayer(window, Helper::loadTexture("assets/snowman_animation.png"), mainCam, {16.f * 100, 16.f * 98.f});

        setupParticles(window, particleManager, mainCam);

        // Bullets
        static sf::Texture bulletTexture;
        bulletTexture.loadFromFile("assets/bullet.png");

        // Particles Texture Setup
        setupParticles(window, particleManager, mainCam);

        // Text Font Setup
        setupTextAndDialogue(window, dialogueManager, mainCam);
        dialogueManager.loadDialoguesFromFile("assets/dialogues/dialogues.txt");

        levelManager.loadLevel(window, GameState::getInstance().getMainCamera(), Constants::STARTING_LEVEL_PATH);

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