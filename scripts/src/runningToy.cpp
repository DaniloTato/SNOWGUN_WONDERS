#include "runningToy.hpp"
#include "Helpers.hpp"
#include "TangibleObject.hpp"

namespace script {

namespace {

struct runningToyState {
  Helper::TriggerOnce trigger1;
  Helper::TriggerOnce trigger2;
  Helper::TriggerOnce trigger3;
  Helper::TriggerOnce trigger4;
};

const float RUNNING_SPEED = 3 * 55.f;
const float JUMP_SPEED = 5 * 60.f;

} // namespace

void runningToy(TangibleObject &tangible, const GeneralContext &ctx) {

  auto &state = tangible.scripter.getState<runningToyState>("runningToy");

  tangible.direction = 1;
  tangible.renderizer.setLayer(1.1);

  tangible.physics.setSpdx(RUNNING_SPEED,
                           PhysicsComponent::SpeedType::MOVEMENT);
  tangible.physics.updateX(tangible.position);

  tangible.collider.horizontalLevelCollision(tangible.position);

  state.trigger4.check((tangible.position.x >= 1710.f), [&tangible] {
    tangible.physics.setSpdy(-JUMP_SPEED,
                             PhysicsComponent::SpeedType::MOVEMENT);
  });

  state.trigger1.check((tangible.position.x >= 1782.f), [&tangible] {
    tangible.physics.setSpdy(-JUMP_SPEED,
                             PhysicsComponent::SpeedType::MOVEMENT);
  });

  state.trigger2.check((tangible.position.x >= 1864.f), [&tangible] {
    tangible.physics.setSpdy(-JUMP_SPEED,
                             PhysicsComponent::SpeedType::MOVEMENT);
  });

  state.trigger3.check((tangible.position.x >= 1934.f), [&tangible] {
    tangible.physics.setSpdy(-JUMP_SPEED,
                             PhysicsComponent::SpeedType::MOVEMENT);
  });

  tangible.physics.updateY(tangible.position);
  if (tangible.collider.verticalLevelCollision(tangible.position)) {
    tangible.physics.setSpdy(0.f, PhysicsComponent::SpeedType::MOVEMENT);
  }
}

} // namespace script