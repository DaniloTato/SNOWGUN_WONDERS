#include "setupMainLevelScene.hpp"

#include "DialogueManager.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "LevelManager.hpp"
#include "ParticleManager.hpp"
#include "ScriptRunner.hpp"
#include "sceneHelperFunctions.hpp"

#include "levelCreatorInputs.hpp"
#include "roomCamera.hpp"

namespace SceneBuilder {

void setupMainLevelScene() {

  LevelManager &levelManager = LevelManager::getInstance();
  DialogueManager &dialogueManager = DialogueManager::getInstance();
  ParticleManager &particleManager = ParticleManager::getInstance();
  GameState &gameState = GameState::getInstance();
  sf::RenderWindow &window = *gameState.getMainWindow();

  setupCameras(gameState);
  auto mainCam = gameState.getMainCamera();
  mainCam->scripter.addScript("roomCamera", script::roomCamera);

  auto *scriptRunner = new ScriptRunner();
  scriptRunner->scripter.addScript("levelCreatorInputs",
                                   script::levelCreatorInputs);

  // Player setup
  TangibleObject *player = createPlayer(
      window,
      Helper::loadTexture(Helper::getPath("assets/snowman_animation.png")),
      mainCam, {16.f * 100, 16.f * 98.f});

  setupParticles(window, particleManager, mainCam);

  // Bullets
  static sf::Texture bulletTexture;
  bulletTexture.loadFromFile(Helper::getPath("assets/bullet.png"));

  // Particles Texture Setup
  setupParticles(window, particleManager, mainCam);

  // Text Font Setup
  setupTextAndDialogue(window, dialogueManager, mainCam);
  dialogueManager.loadDialoguesFromFile(
      (Helper::getPath("assets/dialogues/dialogues.txt")));

  levelManager.loadLevel(window, GameState::getInstance().getMainCamera(),
                         (Helper::getPath("assets/level_data/barracks.json")));

  // context. Imperative
  GeneralContext ctx = {.playerPosition = &(player->position),
                        .textParams = dialogueManager.getAttachedTextParams(),
                        .player = player,
                        .bulletTexture = &bulletTexture};
  GameState::getInstance().updateGeneralContext(ctx);
}
} // namespace SceneBuilder