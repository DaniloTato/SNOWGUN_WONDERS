#include "dramaticZoom.hpp"

#include "Constants.hpp"
#include "GameCamera.hpp"
#include "InputManager.hpp"
#include <cmath>

namespace script {

namespace {
class DramaticZoom {
public:
  enum class State : std::uint8_t { ZoomingIn, Paused, ZoomingOut, Done };

  void start(GameCamera &camera, float zoomInLevel, float pauseDuration) {
    this->zoomInLevel = zoomInLevel;
    this->pauseDuration = pauseDuration;
    this->state = State::ZoomingIn;
    this->timer = 0.f;
    this->originalZoom = camera.getZoom();
    this->impactZoomValue = 0.f;
  }

  void update(GameCamera &camera, const GeneralContext &ctx, float deltaTime) {
    timer += deltaTime;

    switch (state) {
    case State::ZoomingIn:
      impactZoomValue = lerp(impactZoomValue, zoomInLevel - originalZoom, 0.1f);
      if (std::abs(impactZoomValue - (zoomInLevel - originalZoom)) < 0.01f) {
        state = State::Paused;
        timer = 0.f;
      }
      break;

    case State::Paused:
      if (timer >= pauseDuration) {
        state = State::ZoomingOut;
        timer = 0.f;
      }
      break;

    case State::ZoomingOut:
      impactZoomValue = lerp(impactZoomValue, 0.f, 0.1f);
      if (impactZoomValue < 0.01f) {
        impactZoomValue = 0.f;
        state = State::Done;
      }
      break;

    case State::Done:
      impactZoomValue = 0.f;
      break;
    }

    camera.setImpactZoom(impactZoomValue);
  }

  [[nodiscard]] bool isDone() const { return state == State::Done; }

  [[nodiscard]] State getState() const { return state; }

private:
  State state = State::Done;
  float timer = 0.f;
  float pauseDuration = 0.f;
  float zoomInLevel = 1.f;
  float originalZoom = 1.f;
  float impactZoomValue = 0.f;

  float lerp(float a, float b, float t) { return a + t * (b - a); }
};

DramaticZoom dramaticZoomObj;
} // namespace

void dramaticZoom(GameCamera &camera, const GeneralContext &ctx) {
  constexpr float deltaTime = 1.f / Constants::FRAME_RATE;
  dramaticZoomObj.update(camera, ctx, deltaTime);

  if (InputManager::getInstance().isJustPressed("dramaticZoom")) {
    dramaticZoomObj.start(camera, camera.getZoom() * 2.0f, 0.3f);
  }
}

void startDramaticZoom(GameCamera &camera, float zoomInLevel,
                       float pauseDuration) {
  dramaticZoomObj.start(camera, zoomInLevel, pauseDuration);
}

} // namespace script