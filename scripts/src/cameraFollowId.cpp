#include "Constants.hpp"
#include "GameCamera.hpp"
#include "Helpers.hpp"
#include "RuntimeValue.hpp"
#include "SharedMemory.hpp"

namespace script {

namespace {
struct cameraFollowIdState {
  Helper::TriggerOnce firstFrame;
  SharedMemory::Key k_toFollowId;
};
} // namespace

void cameraFollowId(GameCamera &camera, const GeneralContext &ctx) {

  auto &state = camera.scripter.getState<cameraFollowIdState>("cameraFollowId");
  SharedMemory &sharedMemory = SharedMemory::getInstance();

  state.firstFrame.check(true, [&state] {
    state.k_toFollowId = SharedMemory::getInstance().getKey("camera_follow_id");
  });

  Snowlang::RuntimeValue toFollowId = sharedMemory.getField(state.k_toFollowId);
  float zoom = camera.getZoom();

  sf::Vector2f objPosition = {0.f, 0.f};
  sf::Vector2f halfViewSize(Constants::SCREEN_WIDTH / 2.f / zoom,
                            Constants::SCREEN_HEIGHT / 2.f / zoom);

  if (toFollowId.holds_alternative<float>()) {
    float id = std::get<float>(toFollowId.data);
    GameObject *objPtr = GameObject::findGameObjectById(id);
    if (objPtr) {
      objPosition = GameObject::findGameObjectById(id)->position;
    }
  }

  sf::Vector2f desiredCamPos = objPosition - (halfViewSize);
  camera.goTo(desiredCamPos);
}

} // namespace script