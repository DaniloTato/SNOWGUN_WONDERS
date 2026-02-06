#include "titleScreen.hpp"

#include "ColorPalette.hpp"
#include "Constants.hpp"
#include "DialogueManager.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "LevelManager.hpp"
#include "ScriptRunner.hpp"
#include "pressStart.hpp"
#include "sceneHelperFunctions.hpp"
#include <cstddef>

namespace SceneBuilder {

void titleScreen() {

  GameState &gameState = GameState::getInstance();
  setupCameras(gameState);

  auto mainCam = gameState.getMainCamera();
  sf::RenderWindow &window = *gameState.getMainWindow();

  // Particles Texture Setup
  setupParticles(window, ParticleManager::getInstance(), mainCam);

  // Text Font Setup
  DialogueManager &dialogueManager = DialogueManager::getInstance();
  setupTextAndDialogue(window, dialogueManager, mainCam);
  dialogueManager.loadDialoguesFromFile(
      (Helper::getPath("assets/dialogues/dialogues.txt")));

  LevelManager &levelManager = LevelManager::getInstance();

  auto *scriptRunner = new ScriptRunner();
  scriptRunner->scripter.addScript(script::pressStart);

  levelManager.setBackgroundColor(ColorPalette::ElectricBlue);

  sf::Texture dummy;

  // context. Imperative
  GeneralContext ctx = {.playerPosition = {},
                        .textParams = dialogueManager.getAttachedTextParams(),
                        .player = nullptr,
                        .bulletTexture = &dummy};
  GameState::getInstance().updateGeneralContext(ctx);

  sf::Texture &fontTexture =
      Helper::loadTexture((Helper::getPath("assets/font.png")));

  // title text
  RenderizerParameters params{.window = window,
                              .texture = fontTexture,
                              .rect = sf::IntRect(),
                              .position = {0.f, 0.f},
                              .camera = mainCam,
                              .layer = Constants::TEXT_LAYER,
                              .parallax = 1.f};
  auto *title = new GameText(params);
  std::string markupLife =
      "#position 0 90\n"
      "#boundary " +
      std::to_string(Constants::SCREEN_WIDTH / 3) +
      "\n"
      "#alignment center\n" +
      "SNOWGUN\n<color=purple><anim=sin>WONDERS</anim></color>\n";

  title->loadFromMarkup(markupLife);

  auto *enter = new GameText(params);
  std::string markupEnter =
      "#position 0 150\n"
      "#boundary " +
      std::to_string(Constants::SCREEN_WIDTH / 3) +
      "\n"
      "#alignment center\n" +
      "#effect typewriter 0.2\n" +
      "<color=yellow><anim=shake:1>-PRESS ENTER TO START-</anim><color=purple>";

  enter->loadFromMarkup(markupEnter);

  auto *version = new GameText(params);
  std::string markupVersion = "#position 0 30\n"
                              "#boundary " +
                              std::to_string(Constants::SCREEN_WIDTH / 3) +
                              "\n"
                              "#alignment center\n" +
                              "v0.1.0-alpha";

  version->loadFromMarkup(markupVersion);

  LevelManager::getInstance().setBackgroundColor(ColorPalette::Black);
}
} // namespace SceneBuilder