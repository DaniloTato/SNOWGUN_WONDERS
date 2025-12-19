/*SFML dependency*/
#include <SFML/Graphics.hpp>

#include "BulletManager.hpp"
#include "GeneralContext.hpp"

/*Engine Objects*/
#include "GameCamera.hpp"
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "ParticleManager.hpp"
#include "PolyRenderizer.hpp"
#include "RenderableObject.hpp"
#include "Renderizer.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "GameState.hpp"
#include "ScriptRunner.hpp"
#include "DialogueManager.hpp"
#include "EnemyManager.hpp"
#include "SceneManager.hpp"

/*Namespaces*/
#include "Constants.hpp"

/*Camera Scripts*/
#include "cameraAlarm.hpp"
#include "dramaticZoom.hpp"
#include "followPlayer.hpp"
#include "roomCamera.hpp"

/*TangibleObject Scripts*/
#include "reindeer.hpp"
#include "movement.hpp"
#include "particleGeneration.hpp"
#include "reindeer.hpp"
#include "roomCamera.hpp"

/*General Scripts*/
#include "levelCreatorInputs.hpp"
#include "particleGeneration.hpp"
#include "toddTalk.hpp"

/*Blueprints*/
#include "toy.hpp"

#include <cstddef>
#include <cstdlib>
#include <ctime>

void setupLevel2() {
    LevelManager& levelManager = LevelManager::getInstance();
    DialogueManager& dialogueManager = DialogueManager::getInstance();
    ParticleManager& particleManager = ParticleManager::getInstance();
    GameState& gameState = GameState::getInstance();
    sf::RenderWindow& window = *gameState.getMainWindow();

    GameState::getInstance().createCamera(GameState::CameraList::MAIN);
    GameState::getInstance().createCamera(GameState::CameraList::UI);

    GameState::getInstance().getMainCamera()->zoomTo(3.f);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::followPlayer);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::dramaticZoom);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::cameraAlarm);
 
    // Setup Scripts
    ScriptRunner* scriptRunner = new ScriptRunner();
    scriptRunner -> scripter.addScript(script::levelCreatorInputs);
    scriptRunner -> scripter.addScript(script::particleGeneration);

    static sf::Texture toddTexture;
    toddTexture.loadFromFile("assets/todd.png");
    RenderizerParameters toddParams{
        window,
        toddTexture,
        {0,0,16,16},
        {128.f, 0.f},
        gameState.getMainCamera(),
        0.f,
        1.f
    };
    RenderableObject* todd = new RenderableObject(toddParams);
    todd -> scripter.addScript(script::toddTalk);
    dialogueManager.assignDialogue(todd, "Greeting");

    // Player setup
    static sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/snowman_animation.png");
    RenderizerParameters playerParams{
        window,
        playerTexture,
        {0,0,17,17},
        {16.f, 16.f},
        gameState.getMainCamera(),
        -0.1f,
        1.f
    };
    TangibleObject* player = new TangibleObject(playerParams);
    player -> collider.setSize({18.f, 18.f});
    player ->scripter.addScript(script::movement);
    player -> animator.loadAsepriteAnimations("assets/json/snowman_animation.json");
    player -> animator.setSpeedMultiplier(1.8f);

    // Bullets
    static sf::Texture bulletTexture;
    bulletTexture.loadFromFile("assets/bullet.png");

    // Particles Texture Setup
    static sf::Texture particleTexture;
    particleTexture.loadFromFile("assets/particles.png");
    RenderizerParameters particleParams{
        window,
        particleTexture,
        sf::IntRect(0,0,0,0),
        {0.f, 0.f},
        gameState.getMainCamera(),
        -1.f,
        0.7f
    };
    PolyRenderizer* particleRenderizer = new PolyRenderizer(particleParams);
    particleManager.attachPolyRederizer(particleRenderizer);

    // Text Font Setup
    static sf::Texture fontTexture;
    fontTexture.loadFromFile("assets/font.png");
    RenderizerParameters* textParams = new RenderizerParameters{
        window,
        fontTexture,
        sf::IntRect(),
        {0.f, 0.f},
        gameState.getMainCamera(),
        -2.f,
        1.f
    };
    dialogueManager.attachTextParams(textParams);
    dialogueManager.loadDialoguesFromFile("assets/dialogues/dialogues.txt");

    GameText* gt = new GameText(*textParams);
    gt->setFontAtlas(fontTexture, 9, 8, 95, 32);
    gt->loadFromMarkup(*dialogueManager.getDialogue("test2"));

    levelManager.loadLevel(window, GameState::getInstance().getMainCamera(), "assets/level_data/level.json");

    EnemyManager::getInstance().queueCreateEnemy("toy", {16.f,16.f});
    EnemyManager::getInstance().queueCreateEnemy("reindeer", {-100.f,16.f});

    //context. Imperative
    GeneralContext ctx = {
        &(player -> position),
        dialogueManager.getAttachedTextParams(),
        player,
        &bulletTexture
    };
    SceneManager::getInstance().setContext(ctx);
}

void setupMainLevelScene() {
    LevelManager& levelManager = LevelManager::getInstance();
    DialogueManager& dialogueManager = DialogueManager::getInstance();
    ParticleManager& particleManager = ParticleManager::getInstance();
    GameState& gameState = GameState::getInstance();
    sf::RenderWindow& window = *gameState.getMainWindow();

    GameState::getInstance().createCamera(GameState::CameraList::MAIN);
    GameState::getInstance().createCamera(GameState::CameraList::UI);

    GameState::getInstance().getMainCamera()->zoomTo(3.f);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::roomCamera);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::dramaticZoom);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::cameraAlarm);
 
    // Setup Scripts
    ScriptRunner* scriptRunner = new ScriptRunner();
    scriptRunner -> scripter.addScript(script::levelCreatorInputs);
    scriptRunner -> scripter.addScript(script::particleGeneration);

    // Player setup
    static sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/snowman_animation.png");
    RenderizerParameters playerParams{
        window,
        playerTexture,
        {0,0,17,17},
        {16.f * 100, 16.f * 98},
        gameState.getMainCamera(),
        -0.1f,
        1.f
    };
    TangibleObject* player = new TangibleObject(playerParams);
    player -> collider.setSize({18.f, 18.f});
    player ->scripter.addScript(script::movement);
    player -> animator.loadAsepriteAnimations("assets/json/snowman_animation.json");
    player -> animator.setSpeedMultiplier(1.8f);

    // Bullets
    static sf::Texture bulletTexture;
    bulletTexture.loadFromFile("assets/bullet.png");

    levelManager.loadLevel(window, GameState::getInstance().getMainCamera(), Constants::STARTING_LEVEL_PATH);

    // Particles Texture Setup
    static sf::Texture particleTexture;
    particleTexture.loadFromFile("assets/particles.png");
    RenderizerParameters particleParams{
        window,
        particleTexture,
        sf::IntRect(0,0,0,0),
        {0.f, 0.f},
        gameState.getMainCamera(),
        -1.f,
        0.7f
    };
    PolyRenderizer* particleRenderizer = new PolyRenderizer(particleParams);
    particleManager.attachPolyRederizer(particleRenderizer);

    // Text Font Setup
    static sf::Texture fontTexture;
    fontTexture.loadFromFile("assets/font.png");
    RenderizerParameters* textParams = new RenderizerParameters{
        window,
        fontTexture,
        sf::IntRect(),
        {0.f, 0.f},
        gameState.getMainCamera(),
        -2.f,
        1.f
    };
    dialogueManager.attachTextParams(textParams);
    dialogueManager.loadDialoguesFromFile("assets/dialogues/dialogues.txt");

    GameText* gt = new GameText(*textParams);
    gt->setFontAtlas(fontTexture, 9, 8, 95, 32);
    gt->loadFromMarkup(*dialogueManager.getDialogue("test"));

    //context. Imperative
    GeneralContext ctx = {
        &(player -> position),
        dialogueManager.getAttachedTextParams(),
        player,
        &bulletTexture
    };
    SceneManager::getInstance().setContext(ctx);

}

int main() {
    InputManager& inputManager = InputManager::getInstance();
    SceneManager& sceneManager = SceneManager::getInstance();
    EnemyManager& enemyManager = EnemyManager::getInstance();
    GameState& gameState = GameState::getInstance();

    sf::RenderWindow& window = *gameState.getMainWindow();
    window.setFramerateLimit(Constants::FRAME_RATE);

    inputManager.loadBindingsFromJsonFile("./config/control_config.json");

    // Enemy Manager Setup
    enemyManager.loadTexture("toy", "assets/toy.png");
    enemyManager.registerTemplate("toy", blueprint::toy);
    enemyManager.loadTexture("reindeer", "assets/reindeer.png");
    enemyManager.registerTemplate("reindeer", blueprint::reindeer);

    // Scene Manager Setup
    sceneManager.registerScene("barracks", setupMainLevelScene);
    sceneManager.registerScene("level2", setupLevel2);

    sceneManager.loadScene("barracks");

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        inputManager.update();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            inputManager.handleEvent(event);
        }

        if (inputManager.isJustPressed("nextScene")) {
            sceneManager.loadScene("level2");
        }

        sceneManager.update();

        if (!sceneManager.isTransitioning()) {
            LevelManager::getInstance().applyQueuedTileChanges();
            DialogueManager::getInstance().applyQueues();
            BulletManager::getInstance().applyQueues();
            EnemyManager::getInstance().applyQueues();

            BulletManager::getInstance().update();
        }

        window.clear(LevelManager::getInstance().getBackgroundColor());

        for (GameObject* gameObject : GameObject::getGameObjects()) {
            if (gameObject) {
                gameObject->update(SceneManager::getInstance().getContext());
            }
        }

        Renderizer::renderAll();
        window.display();
    }

    return 0;
}