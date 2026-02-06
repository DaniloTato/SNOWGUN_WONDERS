/*SFML dependency*/
#include <SFML/Graphics.hpp>

/*Engine Objects*/
#include "Animator.hpp"
#include "BulletManager.hpp"
#include "CollectableManager.hpp"
#include "DialogueManager.hpp"
#include "EnemyManager.hpp"
#include "GameObject.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include "LevelManager.hpp"
#include "Renderizer.hpp"
#include "SceneManager.hpp"
#include "SoundManager.hpp"
#include "Terminal.hpp"

/*Namespaces*/
#include "Constants.hpp"

/*TangibleObject Scripts*/
#include "missileBlueprint.hpp"
#include "reindeer.hpp"

/*Blueprints*/
#include "chestBlueprint.hpp"
#include "crystalBlueprint.hpp"
#include "healthBlueprint.hpp"
#include "runningToyBlueprint.hpp"
#include "toy.hpp"

/*SceneBuilders*/
#include "Helpers.hpp"
#include "end.hpp"
#include "hills.hpp"
#include "setupLevel2.hpp"
#include "setupMainLevelScene.hpp"
#include "titleScreen.hpp"
#include "tutorial.hpp"

#include <cstddef>
#include <cstdlib>
#include <ctime>

#include <cstdio>
#include <iostream>

using namespace Helper;

int main() {
  InputManager &inputManager = InputManager::getInstance();
  SceneManager &sceneManager = SceneManager::getInstance();
  EnemyManager &enemyManager = EnemyManager::getInstance();
  GameState &gameState = GameState::getInstance();
  CollectableManager &collectableManager = CollectableManager::getInstance();

  gameState.getMainWindow()->setFramerateLimit(Constants::FRAME_RATE);

  inputManager.loadBindingsFromJsonFile(
      (Helper::getPath("config/control_config.json")));

  // Enemy Manager Setup
  enemyManager.textureCache.load(
      "toy", Helper::loadTexture(getPath("assets/toy.png")));
  enemyManager.animationCache.load("toy", Animator::getAsepriteJSONAnimations(
                                              getPath("assets/json/toy.json")));
  enemyManager.registerTemplate("toy", blueprint::toy);

  enemyManager.registerTemplate("runningToy", blueprint::runningToyBlueprint);

  enemyManager.textureCache.load(
      "reindeer", Helper::loadTexture(getPath("assets/reindeer.png")));
  enemyManager.animationCache.load("reindeer",
                                   Animator::getAsepriteJSONAnimations(
                                       getPath("assets/json/reindeer.json")));
  enemyManager.registerTemplate("reindeer", blueprint::reindeer);

  enemyManager.textureCache.load(
      "missile", Helper::loadTexture(getPath("assets/missile.png")));
  enemyManager.animationCache.load(
      "missile",
      Animator::getAsepriteJSONAnimations(getPath("assets/json/missile.json")));
  enemyManager.registerTemplate("missile", blueprint::missileBlueprint);

  // Collectable Manager Setup
  collectableManager.textureCache.load(
      "crystal", Helper::loadTexture(Helper::getPath("assets/crystal.png")));
  collectableManager.animationCache.load(
      "crystal", Animator::getAsepriteJSONAnimations(
                     Helper::getPath("assets/json/crystal.json")));
  collectableManager.registerTemplate("crystal", blueprint::crystalBlueprint);

  collectableManager.textureCache.load(
      "health", Helper::loadTexture(Helper::getPath("assets/ginger.png")));
  collectableManager.animationCache.load(
      "health", Animator::getAsepriteJSONAnimations(
                    Helper::getPath("assets/json/ginger.json")));
  collectableManager.registerTemplate("health", blueprint::healthBlueprint);

  collectableManager.textureCache.load(
      "chest", Helper::loadTexture(Helper::getPath("assets/chest.png")));
  collectableManager.animationCache.load(
      "chest", Animator::getAsepriteJSONAnimations(
                   Helper::getPath("assets/json/chest.json")));
  collectableManager.registerTemplate("chest", blueprint::chestBlueprint);

  // Sound Manager Setup
  SoundManager::getInstance().load(
      "shoot", (Helper::getPath("assets/sounds/buster.wav")));
  SoundManager::getInstance().load(
      "die", (Helper::getPath("assets/sounds/death.wav")));
  SoundManager::getInstance().load(
      "enemyShot", (Helper::getPath("assets/sounds/enemy_shoot.wav")));
  SoundManager::getInstance().load(
      "health", (Helper::getPath("assets/sounds/refill.wav")));
  SoundManager::getInstance().load(
      "crystal", (Helper::getPath("assets/sounds/shard.wav")));
  SoundManager::getInstance().load("hit",
                                   (Helper::getPath("assets/sounds/hit.wav")));
  SoundManager::getInstance().load(
      "crystal2", (Helper::getPath("assets/sounds/shard2.wav")));
  SoundManager::getInstance().load(
      "chest", (Helper::getPath("assets/sounds/chest.wav")));
  SoundManager::getInstance().load(
      "tutorialTune", (Helper::getPath("assets/sounds/tutorial2.wav")));
  SoundManager::getInstance().load(
      "kickHit", (Helper::getPath("assets/sounds/kickHit.wav")));
  SoundManager::getInstance().load(
      "doKick", (Helper::getPath("assets/sounds/doKick.wav")));
  SoundManager::getInstance().load(
      "explosion", (Helper::getPath("assets/sounds/explosion.wav")));
  SoundManager::getInstance().load(
      "enemyDie", (Helper::getPath("assets/sounds/enemyDie.wav")));

  // Scene Manager Setup
  sceneManager.registerScene("barracks", SceneBuilder::setupMainLevelScene);
  sceneManager.registerScene("level2", SceneBuilder::setupLevel2);
  sceneManager.registerScene("tutorial", SceneBuilder::tutorial);
  sceneManager.registerScene("titleScreen", SceneBuilder::titleScreen);
  sceneManager.registerScene("end", SceneBuilder::end);
  sceneManager.registerScene("hills", SceneBuilder::hills);

  sceneManager.loadScene("hills");

  sf::Clock clock;

  while (gameState.getMainWindow() && gameState.getMainWindow()->isOpen()) {
    std::cout << "working \n";
    sf::Event event;
    inputManager.update();

    const std::vector<sf::RenderWindow *> &windows = gameState.getWindows();
    for (WindowTypes i = WindowTypes::MAIN; i != WindowTypes::COUNT;
         i = static_cast<WindowTypes>(static_cast<int>(i) + 1)) {

      auto windowType = static_cast<size_t>(i);
      if (!windows[windowType] || i == WindowTypes::TERMINAL)
        continue;

      while (windows[windowType]->pollEvent(event)) {
        if (i == WindowTypes::MAIN) {
          inputManager.handleEvent(event);
        }

        if (event.type == sf::Event::Closed) {
          GameState::getInstance().removeWindow(i);
          break;
        }
      }
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
      Terminal::destroyKilledTerminals();
    }

    for (WindowTypes i = WindowTypes::MAIN; i != WindowTypes::COUNT;
         i = static_cast<WindowTypes>(static_cast<int>(i) + 1)) {

      auto windowType = static_cast<size_t>(i);
      if (!windows[windowType])
        continue;

      if (i == WindowTypes::MAIN) {
        windows[windowType]->clear(
            LevelManager::getInstance().getBackgroundColor());
      } else {
        windows[windowType]->clear();
      }
    }

    for (GameObject *gameObject : GameObject::getGameObjects()) {
      if (gameObject) {
        gameObject->update(GameState::getInstance().getGeneralContext());
      }
    }

    Renderizer::renderAll();

    for (auto &window : gameState.getWindows()) {
      if (window) {
        window->display();
      }
    }
  }

  return 0;
}