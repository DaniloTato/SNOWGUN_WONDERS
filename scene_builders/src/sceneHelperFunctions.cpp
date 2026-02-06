#include "sceneHelperFunctions.hpp"
#include "AnimatedObject.hpp"
#include "Animator.hpp"
#include "ColorPalette.hpp"
#include "Constants.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "RenderableObject.hpp"
#include "barrelScript.hpp"
#include "cameraAlarm.hpp"
#include "dramaticZoom.hpp"
#include "movement.hpp"

namespace SceneBuilder {

// Creates cameras and adds standard scripts
void setupCameras(GameState &gameState) {
  gameState.createCamera(CameraTypes::MAIN);
  gameState.createCamera(CameraTypes::UI);

  auto mainCam = gameState.getMainCamera();
  mainCam->zoomTo(3.f);
  mainCam->scripter.addScript(script::dramaticZoom);
  mainCam->scripter.addScript(script::cameraAlarm);
}

// Create player object with common setup
TangibleObject *createPlayer(sf::RenderWindow &window, sf::Texture &texture,
                             GameCamera *camera, sf::Vector2f position) {
  RenderizerParameters params{.window = window,
                              .texture = texture,
                              .rect = {0, 0, 17, 17},
                              .position = position,
                              .camera = camera,
                              .layer = -0.1f,
                              .parallax = 1.f};
  auto *player = new TangibleObject(
      params, Animator::getAsepriteJSONAnimations(
                  (Helper::getPath("assets/json/snowman_animation.json"))));
  player->collider.setSize({16.f, 16.f});
  player->collider.setOffset({1.f, 2.f});
  player->scripter.addScript(script::movement);
  player->animator.setSpeedMultiplier(1.8f);

  if (!GameState::getInstance().hasCheckpoint()) {
    GameState::getInstance().setCheckpoint(player->position);
  } else {
    player->position = GameState::getInstance().getCheckpoint();
  }

  return player;
}

// Setup particles
PolyRenderizer *setupParticles(sf::RenderWindow &window,
                               ParticleManager &particleManager,
                               GameCamera *camera) {
  static sf::Texture &particleTexture =
      Helper::loadTexture((Helper::getPath("assets/particles.png")));
  RenderizerParameters params{.window = window,
                              .texture = particleTexture,
                              .rect = sf::IntRect(0, 0, 0, 0),
                              .position = {0.f, 0.f},
                              .camera = camera,
                              .layer = -10.f,
                              .parallax = 0.7f};
  auto *pr = new PolyRenderizer(params);
  particleManager.attachPolyRederizer(pr);
  return pr;
}

// Setup text parameters and dialogue system
RenderizerParameters *setupTextAndDialogue(sf::RenderWindow &window,
                                           DialogueManager &dialogueManager,
                                           GameCamera *camera) {
  static sf::Texture &fontTexture =
      Helper::loadTexture((Helper::getPath("assets/font.png")));
  auto *params = new RenderizerParameters{.window = window,
                                          .texture = fontTexture,
                                          .rect = sf::IntRect(),
                                          .position = {0.f, 0.f},
                                          .camera = camera,
                                          .layer = Constants::TEXT_LAYER,
                                          .parallax = 1.f};
  dialogueManager.attachTextParams(params);
  return params;
}

void ui() {
  GameState &gameState = GameState::getInstance();
  gameState.getUiCamera()->zoomTo(3.f);
  gameState.getUiCamera()->zoomToDesired();

  sf::Texture dummyTexture;
  RenderizerParameters topBarParams{
      .window = *gameState.getMainWindow(),
      .texture = dummyTexture,
      .rect = {0, 0, Constants::SCREEN_WIDTH / 3 + 1, 32},
      .position = {0.f, 0.f},
      .camera = GameState::getInstance().getUiCamera(),
      .layer = Constants::UI_LAYER,
      .parallax = 1.f,
      .registerAsRectShape = true};
  auto *topBarUI = new RenderableObject(topBarParams);
  topBarUI->renderizer.setColor(ColorPalette::Black);

  RenderizerParameters barrelParams{
      .window = *gameState.getMainWindow(),
      .texture =
          Helper::loadTexture((Helper::getPath("assets/whiteBarrel.png"))),
      .rect = {0, 0, 76, 78},
      .position = {-26.f, -26.f},
      .camera = GameState::getInstance().getUiCamera(),
      .layer = Constants::UI_LAYER - 2,
      .parallax = 1.f,
  };
  auto *barrelUI = new AnimatedObject(barrelParams);
  barrelUI->renderizer.setColor(ColorPalette::NeonMagenta);
  barrelUI->animator.loadAsepriteAnimations(
      (Helper::getPath("assets/json/whiteBarrel.json")));
  barrelUI->animator.play("idle_once");
  barrelUI->animator.setSpeedMultiplier(2);
  barrelUI->scripter.addScript(script::barrelScript);

  RenderizerParameters barrelCenterParams{
      .window = *gameState.getMainWindow(),
      .texture =
          Helper::loadTexture((Helper::getPath("assets/barrelCenter.png"))),
      .rect = {0, 0, 21, 21},
      .position = {2.f, 2.f},
      .camera = GameState::getInstance().getUiCamera(),
      .layer = Constants::UI_LAYER - 1,
      .parallax = 1.f,
  };
  new RenderableObject(barrelCenterParams);
}

} // namespace SceneBuilder