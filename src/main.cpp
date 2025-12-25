/*SFML dependency*/
#include <SFML/Graphics.hpp>

#include "BulletManager.hpp"

/*Engine Objects*/
#include "CollectableManager.hpp"
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "Renderizer.hpp"
#include "InputManager.hpp"
#include "GameState.hpp"
#include "DialogueManager.hpp"
#include "EnemyManager.hpp"
#include "SceneManager.hpp"
#include "SoundManager.hpp"

/*Namespaces*/
#include "Constants.hpp"

/*TangibleObject Scripts*/
#include "missileBlueprint.hpp"
#include "reindeer.hpp"

/*Blueprints*/
#include "crystalBlueprint.hpp"
#include "healthBlueprint.hpp"
#include "toy.hpp"
#include "chestBlueprint.hpp"
#include "runningToyBlueprint.hpp"

/*SceneBuilders*/
#include "setupMainLevelScene.hpp"
#include "setupLevel2.hpp"
#include "tutorial.hpp"
#include "titleScreen.hpp"
#include "end.hpp"
#include "Helpers.hpp"

#include <cstddef>
#include <cstdlib>
#include <ctime>

const std::filesystem::path ROOT = Helper::getExecutableDir();

int main() {
    InputManager& inputManager = InputManager::getInstance();
    SceneManager& sceneManager = SceneManager::getInstance();
    EnemyManager& enemyManager = EnemyManager::getInstance();
    GameState& gameState = GameState::getInstance();
    CollectableManager& collectableManager = CollectableManager::getInstance();

    sf::RenderWindow& window = *gameState.getMainWindow();
    window.setFramerateLimit(Constants::FRAME_RATE);

    inputManager.loadBindingsFromJsonFile(ROOT / "config/control_config.json");

    // Enemy Manager Setup
    enemyManager.loadTexture("toy", ROOT / "assets/toy.png");
    enemyManager.registerTemplate("toy", blueprint::toy);
    enemyManager.registerTemplate("runningToy", blueprint::runningToyBlueprint);
    enemyManager.loadTexture("reindeer", ROOT /"assets/reindeer.png");
    enemyManager.registerTemplate("reindeer", blueprint::reindeer);
    enemyManager.loadTexture("missile", ROOT /"assets/missile.png");
    enemyManager.registerTemplate("missile", blueprint::missileBlueprint);

    // Collectable Manager Setup
    collectableManager.loadTexture("crystal", ROOT / "assets/crystal.png");
    collectableManager.registerTemplate("crystal", blueprint::crystalBlueprint);
    collectableManager.loadTexture("health", ROOT / "assets/ginger.png");
    collectableManager.registerTemplate("health", blueprint::healthBlueprint);
    collectableManager.loadTexture("chest", ROOT / "assets/chest.png");
    collectableManager.registerTemplate("chest", blueprint::chestBlueprint);

    // Sound Manager Setup
    SoundManager::getInstance().load("shoot", ROOT / "assets/sounds/buster.wav");
    SoundManager::getInstance().load("die", ROOT / "assets/sounds/death.wav");
    SoundManager::getInstance().load("enemyShot", ROOT / "assets/sounds/enemy_shoot.wav");
    SoundManager::getInstance().load("health", ROOT / "assets/sounds/refill.wav");
    SoundManager::getInstance().load("crystal", ROOT / "assets/sounds/shard.wav");
    SoundManager::getInstance().load("hit", ROOT / "assets/sounds/hit.wav");
    SoundManager::getInstance().load("crystal2", ROOT / "assets/sounds/shard2.wav");
    SoundManager::getInstance().load("chest", ROOT / "assets/sounds/chest.wav");
    SoundManager::getInstance().load("tutorialTune", ROOT / "assets/sounds/tutorial2.wav");
    SoundManager::getInstance().load("kickHit", ROOT / "assets/sounds/kickHit.wav");
    SoundManager::getInstance().load("doKick", ROOT / "assets/sounds/doKick.wav");
    SoundManager::getInstance().load("explosion", ROOT / "assets/sounds/explosion.wav");
    SoundManager::getInstance().load("enemyDie", ROOT / "assets/sounds/enemyDie.wav");

    // Scene Manager Setup
    sceneManager.registerScene("barracks", SceneBuilder::setupMainLevelScene);
    sceneManager.registerScene("level2", SceneBuilder::setupLevel2);
    sceneManager.registerScene("tutorial", SceneBuilder::tutorial);
    sceneManager.registerScene("titleScreen", SceneBuilder::titleScreen);
    sceneManager.registerScene("end", SceneBuilder::end);

    sceneManager.loadScene("titleScreen");

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

        sceneManager.update();
        gameState.updateDt();

        if (!sceneManager.isTransitioning()) {
            LevelManager::getInstance().applyQueuedTileChanges();
            DialogueManager::getInstance().applyQueues();
            BulletManager::getInstance().applyQueues();
            EnemyManager::getInstance().applyQueues();
            CollectableManager::getInstance().applyQueues();
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