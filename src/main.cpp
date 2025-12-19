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
#include "roomCamera.hpp"

/*TangibleObject Scripts*/
#include "reindeer.hpp"
#include "movement.hpp"
#include "particleGeneration.hpp"
#include "reindeer.hpp"
#include "roomCamera.hpp"
#include "tangibleAnimations.hpp"

/*General Scripts*/
#include "levelCreatorInputs.hpp"
#include "particleGeneration.hpp"
#include "toddTalk.hpp"

/*Blueprints*/
#include "toy.hpp"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ctime>

// int main() {

//     InputManager& inputManager = InputManager::getInstance();
//     LevelManager& levelManager = LevelManager::getInstance();
//     GameState& gameState = GameState::getInstance();
//     EnemyManager& enemyManager = EnemyManager::getInstance();

//     sf::RenderWindow& window = *gameState.getMainWindow();
//     window.setFramerateLimit(Constants::FRAME_RATE);

//     sf::Clock clock;

//     inputManager.loadBindingsFromJsonFile("./config/control_config.json");

//     GameState::getInstance().getMainCamera()->zoomTo(3.f);
//     GameState::getInstance().getMainCamera() -> scripter.addScript(script::roomCamera);
//     GameState::getInstance().getMainCamera() -> scripter.addScript(script::dramaticZoom);
//     GameState::getInstance().getMainCamera() -> scripter.addScript(script::cameraAlarm);

//     ScriptRunner scriptRunner;
//     scriptRunner.scripter.addScript(script::levelCreatorInputs);
//     scriptRunner.scripter.addScript(script::particleGeneration);

//     /*Particles*/
//     sf::Texture particleTexture;
//     particleTexture.loadFromFile("assets/particles.png");
//         //as of now. Particles ignore rect, position and parallax
//     RenderizerParameters particleParams{
//         window,
//         particleTexture,
//         sf::IntRect(0,0,0,0),
//         {0.f, 0.f},
//         GameState::getInstance().getMainCamera(),
//         -1.f,
//         0.7f
//     };

//     PolyRenderizer particleRenderizer(particleParams);
//     ParticleManager& particleManager = ParticleManager::getInstance();
//     particleManager.attachPolyRederizer(&particleRenderizer); //Imperative attatchment of rendenderizer.
//     /*Particles*/

//     /*Dialogues*/
//     sf::Texture fontTexture;
//     fontTexture.loadFromFile("assets/font.png");
//     RenderizerParameters textParams{
//         window,
//         fontTexture,
//         sf::IntRect(),
//         {0.f, 0.f},
//         GameState::getInstance().getMainCamera(),
//         -2.f,
//         1.f
//     };
//     DialogueManager& dialogueManager = DialogueManager::getInstance();
//     dialogueManager.attachTextParams(&textParams);
//     dialogueManager.loadDialoguesFromFile("assets/dialogues/dialogues.txt");
//     /*Dialogues*/

//     sf::Texture toddTexture;
//     toddTexture.loadFromFile("assets/todd.png");

//     RenderizerParameters toddParams{
//         window,
//         toddTexture,
//         {0,0,16,16},
//         {128.f, 0.f},
//         GameState::getInstance().getMainCamera(),
//         0.f,
//         1.f
//     };
//     RenderableObject todd(toddParams);
//     todd.scripter.addScript(script::toddTalk);
//     dialogueManager.assignDialogue(&todd, "Greeting");

//     /*Enemy*/
//     enemyManager.loadTexture("toy", "assets/toy.png");
//     enemyManager.registerTemplate("toy", blueprint::toy);
//     //enemyManager.queueCreateEnemy("toy", {16.f,16.f});

//     enemyManager.loadTexture("reindeer", "assets/reindeer.png");
//     enemyManager.registerTemplate("reindeer", blueprint::reindeer);
//     //enemyManager.queueCreateEnemy("reindeer", {-200.f,32.f});
//     /*Enemy*/

//     /*Player*/
//     sf::Texture playerTexture;
//     playerTexture.loadFromFile("assets/snowman_animation.png");

//     RenderizerParameters playerParams{
//         window,
//         playerTexture,
//         {0,0,17,17},
//         {16.f * 100, 16.f * 98},
//         GameState::getInstance().getMainCamera(),
//         -0.1f,
//         1.f
//     };

//     TangibleObject player(playerParams);
//     player.collider.setSize({18.f, 18.f});

//     player.scripter.addScript(script::tangibleAnimations);
//     player.scripter.addScript(script::movement);

//     player.animator.loadAsepriteAnimations("assets/json/snowman_animation.json");
//     player.animator.setSpeedMultiplier(1.8f);
//     /*Player*/

//     /*Bullet*/
//     sf::Texture bulletTexture;
//     bulletTexture.loadFromFile("assets/bullet.png");
//     BulletManager& bulletManager = BulletManager::getInstance();
//     /*Bullet*/

//     levelManager.loadLevel(window, GameState::getInstance().getMainCamera(), Constants::STARTING_LEVEL_PATH);

//     while (window.isOpen()) {
//         sf::Event event;

//         inputManager.update();
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();

//             inputManager.handleEvent(event);
//         }

//         //yet to modify general context
//         GeneralContext ctx = {
//             player.position,
//             window,
//             textParams,
//             &player,
//             bulletTexture
//         };

//         levelManager.applyQueuedTileChanges();
//         dialogueManager.applyQueues();
//         bulletManager.getInstance().applyQueues();
//         enemyManager.getInstance().applyQueues();
        
//         bulletManager.update();

//         window.clear(levelManager.getBackgroundColor());

//         for (GameObject* gameObject :  GameObject::getGameObjects()) {
//             if (!gameObject) {
//                 std::cerr << "[Warning] Null GameObject pointer encountered during update. Skipping.\n";
//                 continue;
//             }
//             gameObject->update(ctx);
//         }

//         Renderizer::renderAll();

//         window.display();
//     }
    
//     return 0;
// }

void setupMainLevelScene() {
    LevelManager& levelManager = LevelManager::getInstance();
    EnemyManager& enemyManager = EnemyManager::getInstance();
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

    // Particles
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
    static PolyRenderizer particleRenderizer(particleParams);
    particleManager.attachPolyRederizer(&particleRenderizer);

    static sf::Texture fontTexture;
    fontTexture.loadFromFile("assets/font.png");
    RenderizerParameters textParams{
        window,
        fontTexture,
        sf::IntRect(),
        {0.f, 0.f},
        gameState.getMainCamera(),
        -2.f,
        1.f
    };
    dialogueManager.attachTextParams(&textParams);
    dialogueManager.loadDialoguesFromFile("assets/dialogues/dialogues.txt");

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

    //context. Imperative
    GeneralContext ctx = {
        &(player -> position),
        &textParams,
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

    sceneManager.registerScene("MainLevel", setupMainLevelScene);

    // Start the first scene explicitly
    sceneManager.loadScene("MainLevel");

    sf::Clock clock;

    enemyManager.loadTexture("toy", "assets/toy.png");
    enemyManager.registerTemplate("toy", blueprint::toy);
    enemyManager.loadTexture("reindeer", "assets/reindeer.png");
    enemyManager.registerTemplate("reindeer", blueprint::reindeer);

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
            sceneManager.loadScene("MainLevel");
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