#include "setupLevel2.hpp"

#include "ColorPalette.hpp"
#include "DialogueManager.hpp"
#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "LevelManager.hpp"
#include "ParticleManager.hpp"
#include "ScriptRunner.hpp"
#include "sceneHelperFunctions.hpp"

#include "Helpers.hpp"
#include "followPlayer.hpp"
#include "levelCreatorInputs.hpp"
#include "particleGeneration.hpp"
#include "toddTalk.hpp"

namespace SceneBuilder {

void setupLevel2() {
  auto &levelManager = LevelManager::getInstance();
  auto &dialogueManager = DialogueManager::getInstance();
  auto &particleManager = ParticleManager::getInstance();
  auto &gameState = GameState::getInstance();
  auto &window = *gameState.getMainWindow();

  setupCameras(gameState);
  auto mainCam = gameState.getMainCamera();
  mainCam->scripter.addScript(script::followPlayer);

  auto *scriptRunner = new ScriptRunner();
  scriptRunner->scripter.addScript(script::levelCreatorInputs);
  scriptRunner->scripter.addScript(script::particleGeneration);

  // Todd setup
  static sf::Texture &toddTexture =
      Helper::loadTexture(Helper::getPath("assets/todd.png"));
  RenderizerParameters toddParams{
      window, toddTexture, {0, 0, 16, 16}, {128.f, 0.f}, mainCam, 0.f, 1.f};
  auto *todd = new RenderableObject(toddParams);
  todd->scripter.addScript(script::toddTalk);
  dialogueManager.assignDialogue(todd, "Greeting");

  // Player
  TangibleObject *player =
      createPlayer(window, Helper::loadTexture("assets\\snowman_animation.png"),
                   mainCam, {16.f, 16.f});

  setupParticles(window, particleManager, mainCam);

  setupTextAndDialogue(window, dialogueManager, mainCam);
  dialogueManager.loadDialoguesFromFile("assets\\dialogues\\dialogues.txt");

  levelManager.loadLevel(window, mainCam, "assets\\level_data\\level.json");

  EnemyManager::getInstance().queueCreateEnemy("toy", {16.f, 16.f});
  EnemyManager::getInstance().queueCreateEnemy("reindeer", {-100.f, 16.f});

  LevelManager::getInstance().setBackgroundColor(ColorPalette::ElectricBlue);

  GeneralContext ctx{&(player->position),
                     dialogueManager.getAttachedTextParams(), player,
                     &Helper::loadTexture("assets\\bullet.png")};
  GameState::getInstance().updateGeneralContext(ctx);
}

} // namespace SceneBuilder