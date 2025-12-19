#include "cameraAlarm.hpp"
#include "ColorPalette.hpp"
#include "GameCamera.hpp"
#include "cameraShake.hpp"
#include "Constants.hpp"
#include "GameState.hpp"
#include "RenderableObject.hpp"
#include "InputManager.hpp"
#include <memory>

namespace script{

    namespace{
        const float ALARM_FREQUENCY = 2.f;
        const float SCREENSHAKE_DURATION = 1.f;
        const float SCREENSHAKE_INTENSITY = 30.f;
        const float LERP_SPEED = 5.f;

        class AlarmState {
        public:
            void ToggleAlarm(){
                on = !on;
            }

            bool isOn(){
                return on;
            }

            void init(sf::RenderWindow& window) {
                if (initialized) return;

                sf::Texture dummyTexture;

                RenderizerParameters params{
                    window,
                    dummyTexture,
                    {0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT},
                    {0.f, 0.f},
                    GameState::getInstance().getUiCamera(),
                    -100.f,
                    1.f,
                    true
                };

                overlay = std::make_unique<RenderableObject>(params);
                overlay->renderizer.setColor(sf::Color(255, 0, 0, 0));

                initialized = true;
            }

            void update(bool isShaking) {
                float targetAlpha = isShaking ? 100.f : 0.f;

                currentAlpha += (targetAlpha - currentAlpha) * LERP_SPEED * GameState::getInstance().dt();
                if (currentAlpha < 0.f) currentAlpha = 0.f;
                if (currentAlpha > 100.f) currentAlpha = 100.f;

                setAlpha(static_cast<sf::Uint8>(currentAlpha));
            }

            void setAlpha(sf::Uint8 alpha) {
                overlay->renderizer.setColor(
                    sf::Color(
                        ColorPalette::MexicanPink.r, 
                        ColorPalette::MexicanPink.g,
                        ColorPalette::MexicanPink.b, 
                        alpha));
            }

            float alarmCountdown = 0;

        private:
            bool on = false;
            bool initialized = false;
            std::shared_ptr<RenderableObject> overlay;
            float currentAlpha = 0.f;
        };
    }

    void cameraAlarm(GameCamera& camera, const GeneralContext& ctx) {
        cameraShake(camera, ctx);

        auto& stateAny = camera.scripter.scriptState["cameraAlarm"];
        if (!stateAny.has_value()) {
            stateAny = AlarmState{};
        }
        auto& state = std::any_cast<AlarmState&>(stateAny);

        state.init(*GameState::getInstance().getMainWindow());

        state.update(script::ShakeFunctions::isShaking(camera));

        if(InputManager::getInstance().isJustPressed("startAlarm")){
            state.ToggleAlarm();
        }

        state.alarmCountdown -= GameState::getInstance().dt();

        if(state.isOn() && state.alarmCountdown <= 0){
            script::ShakeFunctions::startShake(camera, SCREENSHAKE_DURATION, SCREENSHAKE_INTENSITY);
            state.alarmCountdown = ALARM_FREQUENCY;
        }
    }

}