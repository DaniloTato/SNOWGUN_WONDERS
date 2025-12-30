#include <random>

#include "cameraShake.hpp"

#include "GameCamera.hpp"
#include "GameState.hpp"

namespace script {

namespace {
class ScreenShake {
public:
  ScreenShake() : generator(std::random_device{}()), distribution(-1.f, 1.f) {}

  void start(float duration, float intensity) {
    shakeDuration = duration;
    shakeIntensity = intensity;
    elapsed = 0.f;
  }

  void update(GameCamera &camera, const GeneralContext &ctx, float deltaTime) {
    if (shakeDuration <= 0.f)
      return;

    elapsed += deltaTime;
    if (elapsed >= shakeDuration) {
      shakeDuration = 0.f;
      camera.setCameraShakePosition({0.f, 0.f});
      return;
    }

    float offsetX = distribution(generator) * shakeIntensity;
    float offsetY = distribution(generator) * shakeIntensity;

    camera.setCameraShakePosition({offsetX, offsetY});
  }

  [[nodiscard]] const float &getShakeDuration() const { return shakeDuration; }

private:
  float shakeDuration = 0.f;
  float shakeIntensity = 0.f;
  float elapsed = 0.f;
  std::mt19937 generator;
  std::uniform_real_distribution<float> distribution;
};
} // namespace

namespace ShakeFunctions {
bool isShaking(GameCamera &camera) {
  auto it = camera.scripter.scriptState.find("cameraShake");
  if (it == camera.scripter.scriptState.end())
    return false;
  const auto &state = std::any_cast<const ScreenShake &>(it->second);
  return state.getShakeDuration() != 0.f;
}

void startShake(GameCamera &camera, float duration, float intensity) {
  auto it = camera.scripter.scriptState.find("cameraShake");
  if (it == camera.scripter.scriptState.end())
    return;
  auto &state = std::any_cast<ScreenShake &>(it->second);
  state.start(duration, intensity);
}
} // namespace ShakeFunctions

void cameraShake(GameCamera &camera, const GeneralContext &ctx) {
  auto &state = camera.scripter.getState<ScreenShake>("cameraShake");

  state.update(camera, ctx, GameState::getInstance().dt());
}

} // namespace script