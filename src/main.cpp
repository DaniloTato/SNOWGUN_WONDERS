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

#include <cstddef>
#include <cstdlib>
#include <ctime>

int main() {
    InputManager& inputManager = InputManager::getInstance();
    SceneManager& sceneManager = SceneManager::getInstance();
    EnemyManager& enemyManager = EnemyManager::getInstance();
    GameState& gameState = GameState::getInstance();
    CollectableManager& collectableManager = CollectableManager::getInstance();

    sf::RenderWindow& window = *gameState.getMainWindow();
    window.setFramerateLimit(Constants::FRAME_RATE);

    inputManager.loadBindingsFromJsonFile("./config/control_config.json");

    // Enemy Manager Setup
    enemyManager.loadTexture("toy", "assets/toy.png");
    enemyManager.registerTemplate("toy", blueprint::toy);
    enemyManager.registerTemplate("runningToy", blueprint::runningToyBlueprint);
    enemyManager.loadTexture("reindeer", "assets/reindeer.png");
    enemyManager.registerTemplate("reindeer", blueprint::reindeer);
    enemyManager.loadTexture("missile", "assets/missile.png");
    enemyManager.registerTemplate("missile", blueprint::missileBlueprint);

    // Collectable Manager Setup
    collectableManager.loadTexture("crystal", "assets/crystal.png");
    collectableManager.registerTemplate("crystal", blueprint::crystalBlueprint);
    collectableManager.loadTexture("health", "assets/chest.png");
    collectableManager.registerTemplate("health", blueprint::healthBlueprint);
    collectableManager.loadTexture("chest", "assets/chest.png");
    collectableManager.registerTemplate("chest", blueprint::chestBlueprint);

    // Scene Manager Setup
    sceneManager.registerScene("barracks", SceneBuilder::setupMainLevelScene);
    sceneManager.registerScene("level2", SceneBuilder::setupLevel2);
    sceneManager.registerScene("tutorial", SceneBuilder::tutorial);

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
            sceneManager.loadScene("tutorial");
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