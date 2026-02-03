#include "cameraAlarm.hpp"
#include "ColorPalette.hpp"
#include "Constants.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include "RenderableObject.hpp"
#include "cameraShake.hpp"
#include <memory>

namespace script {

namespace {
const float ALARM_FREQUENCY = 2.f;
const float SCREENSHAKE_DURATION = 1.f;
const float SCREENSHAKE_INTENSITY = 30.f;
const float LERP_SPEED = 5.f;

class AlarmState {
public:
  void ToggleAlarm() { on = !on; }

  bool isOn() { return on; }

  void init(sf::RenderWindow &window) {
    if (initialized)
      return;

    sf::Texture dummyTexture;

    RenderizerParameters params{
        .window = window,
        .texture = dummyTexture,
        .rect = {0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT},
        .position = {0.f, 0.f},
        .camera = GameState::getInstance().getUiCamera(),
        .layer = -100.f,
        .parallax = 1.f,
        .registerAsRectShape = true};

    overlay = std::make_unique<RenderableObject>(params);
    overlay->renderizer.setColor(sf::Color(255, 0, 0, 0));

    initialized = true;
  }

  void update(bool isShaking) {
    float targetAlpha = isShaking && on ? 100.f : 0.f;

    currentAlpha += (targetAlpha - currentAlpha) * LERP_SPEED *
                    GameState::getInstance().dt();
    if (currentAlpha < 0.f)
      currentAlpha = 0.f;
    if (currentAlpha > 100.f)
      currentAlpha = 100.f;

    setAlpha(static_cast<sf::Uint8>(currentAlpha));
  }

  void setAlpha(sf::Uint8 alpha) {
    overlay->renderizer.setColor(sf::Color(ColorPalette::MexicanPink.r,
                                           ColorPalette::MexicanPink.g,
                                           ColorPalette::MexicanPink.b, alpha));
  }

  float alarmCountdown = 0;

private:
  bool on = false;
  bool initialized = false;
  std::shared_ptr<RenderableObject> overlay;
  float currentAlpha = 0.f;
};
} // namespace

void cameraAlarm(GameCamera &camera, const GeneralContext &ctx) {
  cameraShake(camera, ctx);

  auto &state = camera.scripter.getState<AlarmState>("cameraAlarm");

  state.init(*GameState::getInstance().getMainWindow());

  state.update(script::ShakeFunctions::isShaking(camera));

  if (InputManager::getInstance().isJustPressed("startAlarm")) {
    state.ToggleAlarm();
  }

  state.alarmCountdown -= GameState::getInstance().dt();

  if (state.isOn() && state.alarmCountdown <= 0) {
    script::ShakeFunctions::startShake(camera, SCREENSHAKE_DURATION,
                                       SCREENSHAKE_INTENSITY);
    state.alarmCountdown = ALARM_FREQUENCY;
  }
}

} // namespace script