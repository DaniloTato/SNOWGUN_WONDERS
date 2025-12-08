#include "GameCamera.hpp"

namespace script {

    class DramaticZoom {
    public:
        void start(GameCamera& camera, float zoomInLevel, float pauseDuration) {
            this->zoomInLevel = zoomInLevel;
            this->pauseDuration = pauseDuration;
            this->state = State::ZoomingIn;
            this->timer = 0.f;
            this->originalZoom = camera.getZoom();
        }

        void update(GameCamera& camera, const CameraContext& ctx, float deltaTime) {
            timer += deltaTime;

            switch(state) {
                case State::ZoomingIn:
                    camera.zoomTo(lerp(camera.getZoom(), zoomInLevel, 0.1f));
                    if (abs(camera.getZoom() - zoomInLevel) < 0.01f) {
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
                    camera.zoomTo(lerp(camera.getZoom(), originalZoom, 0.1f));
                    if (abs(camera.getZoom() - originalZoom) < 0.01f) {
                        state = State::Done;
                    }
                    break;

                case State::Done:
                    break;
            }
            camera.goTo(ctx.position);
        }

        bool isDone() const {
            return state == State::Done;
        }

    private:
        enum class State { ZoomingIn, Paused, ZoomingOut, Done } state = State::Done;
        float timer = 0.f;
        float pauseDuration = 0.f;
        float zoomInLevel = 1.f;
        float originalZoom = 1.f;

        float lerp(float a, float b, float t) {
            return a + t * (b - a);
        }
    };

    static DramaticZoom dramaticZoomObj;

    void dramaticZoom(GameCamera& camera, const CameraContext& ctx) {
        constexpr float deltaTime = 1.f / 60.f;
        dramaticZoomObj.update(camera, ctx, deltaTime);
    }

}