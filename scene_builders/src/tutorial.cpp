#include "tutorial.hpp"

#include "AnimatedObject.hpp"
#include "CollectableManager.hpp"
#include "ColorPalette.hpp"
#include "Constants.hpp"
#include "DialogueManager.hpp"
#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "LevelManager.hpp"
#include "ParticleManager.hpp"
#include "SceneManager.hpp"
#include "ScriptRunner.hpp"
#include "barrelScript.hpp"
#include "followPlayer.hpp"
#include "sceneHelperFunctions.hpp"

#include "levelCreatorInputs.hpp"
#include "particleGeneration.hpp"
#include "tutorialTriggers.hpp"
#include "updateCrystalCounterScript.hpp"
#include "updateLifeCounterScript.hpp"

#include <iostream>

namespace SceneBuilder {

void ui() {
  GameState &gameState = GameState::getInstance();
  gameState.getUiCamera()->zoomTo(3.f);
  gameState.getUiCamera()->zoomToDesired();

  sf::Texture dummyTexture;
  RenderizerParameters topBarParams{*gameState.getMainWindow(),
                                    dummyTexture,
                                    {0, 0, Constants::SCREEN_WIDTH / 3 + 1, 32},
                                    {0.f, 0.f},
                                    GameState::getInstance().getUiCamera(),
                                    Constants::UI_LAYER,
                                    1.f,
                                    true};
  auto *topBarUI = new RenderableObject(topBarParams);
  topBarUI->renderizer.setColor(ColorPalette::Black);

  RenderizerParameters barrelParams{
      *gameState.getMainWindow(),
      Helper::loadTexture((Helper::getPath("assets/whiteBarrel.png"))),
      {0, 0, 76, 78},
      {-26.f, -26.f},
      GameState::getInstance().getUiCamera(),
      Constants::UI_LAYER - 2,
      1.f,
  };
  auto *barrelUI = new AnimatedObject(barrelParams);
  barrelUI->renderizer.setColor(ColorPalette::NeonMagenta);
  barrelUI->animator.loadAsepriteAnimations(
      (Helper::getPath("assets/json/whiteBarrel.json")));
  barrelUI->animator.play("idle_once");
  barrelUI->animator.setSpeedMultiplier(2);
  barrelUI->scripter.addScript(script::barrelScript);

  RenderizerParameters barrelCenterParams{
      *gameState.getMainWindow(),
      Helper::loadTexture((Helper::getPath("assets/barrelCenter.png"))),
      {0, 0, 21, 21},
      {2.f, 2.f},
      GameState::getInstance().getUiCamera(),
      Constants::UI_LAYER - 1,
      1.f,
  };
  new RenderableObject(barrelCenterParams);
}

void tutorial() {

  GameState &gameState = GameState::getInstance();
  setupCameras(gameState);

  auto mainCam = gameState.getMainCamera();
  sf::RenderWindow &window = *gameState.getMainWindow();

  // Text Font Setup
  DialogueManager &dialogueManager = DialogueManager::getInstance();
  setupTextAndDialogue(window, dialogueManager, mainCam);
  dialogueManager.loadDialoguesFromFile(
      (Helper::getPath("assets/dialogues/dialogues.txt")));
  dialogueManager.printByKey("kickTutorial");

  ui();

  LevelManager &levelManager = LevelManager::getInstance();
  ParticleManager &particleManager = ParticleManager::getInstance();
  mainCam->scripter.addScript(script::followPlayer);

  auto *scriptRunner = new ScriptRunner();
  scriptRunner->scripter.addScript(script::levelCreatorInputs);
  scriptRunner->scripter.addScript(script::particleGeneration);
  scriptRunner->scripter.addScript(script::tutorialTriggers);
  scriptRunner->scripter.addScript(script::updateLifeCounterScript);
  scriptRunner->scripter.addScript(script::updateCrystalCounterScript);

  // Player setup
  TangibleObject *player = createPlayer(
      window,
      Helper::loadTexture((Helper::getPath("assets/snowman_animation.png"))),
      mainCam, {16.f * 100, 16.f * 99.f});

  // Bullets
  static sf::Texture bulletTexture;
  bulletTexture.loadFromFile((Helper::getPath("assets/bullet.png")));
  // Particles Texture Setup
  setupParticles(window, particleManager, mainCam);

  levelManager.loadLevel(window, GameState::getInstance().getMainCamera(),
                         (Helper::getPath("assets/level_data/tutorial.json")));

  levelManager.setBackgroundColor(ColorPalette::ElectricBlue);

  CollectableManager::getInstance().queueCreateCollectable("chest",
                                                           {124 * 16, 99 * 16});
  CollectableManager::getInstance().queueCreateCollectable("chest",
                                                           {138 * 16, 98 * 16});
  CollectableManager::getInstance().queueCreateCollectable("chest",
                                                           {384 * 16, 76 * 16});
  CollectableManager::getInstance().queueCreateCollectable("chest",
                                                           {420 * 16, 88 * 16});
  CollectableManager::getInstance().queueCreateCollectable("chest",
                                                           {374 * 16, 82 * 16});
  CollectableManager::getInstance().queueCreateCollectable("chest",
                                                           {234 * 16, 91 * 16});
  CollectableManager::getInstance().queueCreateCollectable("health",
                                                           {211 * 16, 95 * 16});

  EnemyManager::getInstance().loadSpawnDefinitionsFromJson(
      (Helper::getPath("assets/level_data/tutorialEnemies.json")));

  // context. Imperative
  GeneralContext ctx = {&(player->position),
                        dialogueManager.getAttachedTextParams(), player,
                        &bulletTexture};
  SceneManager::getInstance().setContext(ctx);

  /*Force excecution of camera script to follow player so the camera can go to
  the desired position instantly*/
  mainCam->zoomTo(3.0f);
  mainCam->zoomToDesired();
  script::followPlayer(*mainCam, ctx);
  mainCam->goToDesired();

  static sf::Music tutorialMusic;
  static bool musicStarted = false;

  if (!musicStarted) {
    if (!tutorialMusic.openFromFile(
            (Helper::getPath("assets/sounds/tutorial2.wav")))) {
      std::cerr << "Failed to load tutorial music\n";
    } else {
      tutorialMusic.setLoop(true);
      tutorialMusic.setVolume(100.f);
      tutorialMusic.play();
      musicStarted = true;
    }
  }

  GameState::getInstance().changePlayerHealth(
      3 - GameState::getInstance().getPlayerHealth());
}
} // namespace SceneBuilder