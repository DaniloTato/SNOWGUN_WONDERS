#include <random>

#include "GameCamera.hpp"


namespace script {

    class ScreenShake {
    public:
        ScreenShake()
            : shakeDuration(0.f), shakeIntensity(0.f), generator(std::random_device{}()),
              distribution(-1.f, 1.f) {}

        void start(float duration, float intensity) {
            shakeDuration = duration;
            shakeIntensity = intensity;
            elapsed = 0.f;
        }

        void update(GameCamera& camera, const CameraContext& ctx, float deltaTime) {
            if (shakeDuration <= 0.f) return;

            elapsed += deltaTime;
            if (elapsed >= shakeDuration) {
                shakeDuration = 0.f;
                camera.goTo(ctx.position);
                return;
            }

            float offsetX = distribution(generator) * shakeIntensity;
            float offsetY = distribution(generator) * shakeIntensity;

            camera.goTo(ctx.position + sf::Vector2f(offsetX, offsetY));
        }

    private:
        float shakeDuration;
        float shakeIntensity;
        float elapsed = 0.f;
        std::mt19937 generator;
        std::uniform_real_distribution<float> distribution;
    };

    static ScreenShake screenShake;

    void cameraShake(GameCamera& camera, const CameraContext& ctx) {
        constexpr float deltaTime = 1.f / 60.f;
        screenShake.update(camera, ctx, deltaTime);
    }

}