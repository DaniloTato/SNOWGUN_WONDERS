#include "hills.hpp"
#include "ColorPalette.hpp"
#include "DialogueManager.hpp"
#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "LevelManager.hpp"
#include "ParticleManager.hpp"
#include "ScriptRunner.hpp"
#include "followPlayer.hpp"
#include "levelCreatorInputs.hpp"
#include "sceneHelperFunctions.hpp"
#include "updateCrystalCounterScript.hpp"
#include "updateLifeCounterScript.hpp"

namespace SceneBuilder {

void hills() {
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
                         (Helper::getPath("assets/level_data/hills.json")));

  levelManager.setBackgroundColor(ColorPalette::PeachCream);

  EnemyManager::getInstance().loadSpawnDefinitionsFromJson(
      (Helper::getPath("assets/level_data/tutorialEnemies.json")));

  // context. Imperative
  GeneralContext ctx = {.playerPosition = &(player->position),
                        .textParams = dialogueManager.getAttachedTextParams(),
                        .player = player,
                        .bulletTexture = &bulletTexture};
  GameState::getInstance().updateGeneralContext(ctx);

  /*Force excecution of camera script to follow player so the camera can go to
  the desired position instantly*/
  mainCam->zoomTo(3.0f);
  mainCam->zoomToDesired();
  script::followPlayer(*mainCam, ctx);
  mainCam->goToDesired();

  GameState::getInstance().changePlayerHealth(
      3 - GameState::getInstance().getPlayerHealth());
}

} // namespace SceneBuilder