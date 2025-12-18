#include <random>

#include "cameraShake.hpp"

#include "GameCamera.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"

namespace script {

    namespace{
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

            void update(GameCamera& camera, const GeneralContext& ctx, float deltaTime) {
                if (shakeDuration <= 0.f) return;

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

            const float& getShakeDuration() const{
                return shakeDuration;
            }

        private:
            float shakeDuration;
            float shakeIntensity;
            float elapsed = 0.f;
            std::mt19937 generator;
            std::uniform_real_distribution<float> distribution;
        };
    }

    namespace ShakeFunctions{
        bool isShaking(GameCamera& camera){
            auto it = camera.scripter.scriptState.find("damageable");
            if (it == camera.scripter.scriptState.end()) return false;

            const auto& state = std::any_cast<const ScreenShake&>(it->second);
            return state.getShakeDuration() != 0.f;
        }
    }

    void cameraShake(GameCamera& camera, const GeneralContext& ctx) {
        auto& stateAny = camera.scripter.scriptState["cameraShake"];
        if (!stateAny.has_value()) {
            stateAny = ScreenShake{};
        }
        auto& state = std::any_cast<ScreenShake&>(stateAny);

        state.update(camera, ctx, GameState::getInstance().dt());

        if(InputManager::getInstance().isJustPressed("shakeCamera")){
            state.start(0.5f, 40.f);
        }
    }

}