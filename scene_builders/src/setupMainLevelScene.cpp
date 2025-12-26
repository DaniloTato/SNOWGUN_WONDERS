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
#include "Helpers.hpp"

const std::filesystem::path ROOT = Helper::getExecutableDir();

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
        TangibleObject* player = createPlayer(window, Helper::loadTexture((ROOT / "assets\\snowman_animation.png").string()), mainCam, {16.f * 100, 16.f * 98.f});

        setupParticles(window, particleManager, mainCam);

        // Bullets
        static sf::Texture bulletTexture;
        bulletTexture.loadFromFile((ROOT / "assets\\bullet.png").string());

        // Particles Texture Setup
        setupParticles(window, particleManager, mainCam);

        // Text Font Setup
        setupTextAndDialogue(window, dialogueManager, mainCam);
        dialogueManager.loadDialoguesFromFile((ROOT / "assets\\dialogues\\dialogues.txt").string());

        levelManager.loadLevel(window, GameState::getInstance().getMainCamera(), (ROOT / "assets\\level_data\\barracks.json").string());

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