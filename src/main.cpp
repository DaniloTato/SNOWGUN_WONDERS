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

using namespace Helper;

int main() {
    InputManager& inputManager = InputManager::getInstance();
    SceneManager& sceneManager = SceneManager::getInstance();
    EnemyManager& enemyManager = EnemyManager::getInstance();
    GameState& gameState = GameState::getInstance();
    CollectableManager& collectableManager = CollectableManager::getInstance();

    sf::RenderWindow& window = *gameState.getMainWindow();
    window.setFramerateLimit(Constants::FRAME_RATE);

    inputManager.loadBindingsFromJsonFile((Helper::getPath("config/control_config.json")));

    // Enemy Manager Setup
    enemyManager.loadTexture("toy", (getPath("assets/toy.png")));
    enemyManager.cacheAnimations("toy", Animator::getAsepriteJSONAnimations(getPath("assets/json/toy.json")));
    enemyManager.registerTemplate("toy", blueprint::toy);
    enemyManager.registerTemplate("runningToy", blueprint::runningToyBlueprint);
    enemyManager.loadTexture("reindeer", (getPath("assets/reindeer.png")));
    enemyManager.cacheAnimations("reindeer", Animator::getAsepriteJSONAnimations(getPath("assets/json/reindeer.json")));
    enemyManager.registerTemplate("reindeer", blueprint::reindeer);
    enemyManager.loadTexture("missile", (getPath("assets/missile.png")));
    enemyManager.cacheAnimations("missile", Animator::getAsepriteJSONAnimations(getPath("assets/json/missile.json")));
    enemyManager.registerTemplate("missile", blueprint::missileBlueprint);

    // Collectable Manager Setup
    collectableManager.loadTexture("crystal", (Helper::getPath("assets/crystal.png")));
    collectableManager.registerTemplate("crystal", blueprint::crystalBlueprint);
    collectableManager.loadTexture("health", (Helper::getPath("assets/ginger.png")));
    collectableManager.registerTemplate("health", blueprint::healthBlueprint);
    collectableManager.loadTexture("chest", (Helper::getPath("assets/chest.png")));
    collectableManager.registerTemplate("chest", blueprint::chestBlueprint);

    // Sound Manager Setup
    SoundManager::getInstance().load("shoot", (Helper::getPath("assets/sounds/buster.wav")));
    SoundManager::getInstance().load("die", (Helper::getPath("assets/sounds/death.wav")));
    SoundManager::getInstance().load("enemyShot", (Helper::getPath("assets/sounds/enemy_shoot.wav")));
    SoundManager::getInstance().load("health", (Helper::getPath("assets/sounds/refill.wav")));
    SoundManager::getInstance().load("crystal", (Helper::getPath("assets/sounds/shard.wav")));
    SoundManager::getInstance().load("hit", (Helper::getPath("assets/sounds/hit.wav")));
    SoundManager::getInstance().load("crystal2", (Helper::getPath("assets/sounds/shard2.wav")));
    SoundManager::getInstance().load("chest", (Helper::getPath("assets/sounds/chest.wav")));
    SoundManager::getInstance().load("tutorialTune", (Helper::getPath("assets/sounds/tutorial2.wav")));
    SoundManager::getInstance().load("kickHit", (Helper::getPath("assets/sounds/kickHit.wav")));
    SoundManager::getInstance().load("doKick", (Helper::getPath("assets/sounds/doKick.wav")));
    SoundManager::getInstance().load("explosion", (Helper::getPath("assets/sounds/explosion.wav")));
    SoundManager::getInstance().load("enemyDie", (Helper::getPath("assets/sounds/enemyDie.wav")));

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