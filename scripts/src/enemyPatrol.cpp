#include "enemyPatrol.hpp"
#include "damageable.hpp"

namespace script {

void enemyPatrol(TangibleObject &tangible, const GeneralContext &ctx) {

  script::damageable(tangible, ctx, 10, 0.6f, 0.65f);

  bool beingHurt = script::DamageFunctions::isBeingHurt(tangible);

  if (script::DamageFunctions::isDying(tangible)) {
    return;
  }

  if (!beingHurt) {
    tangible.physics.setSpdx(static_cast<float>(tangible.direction),
                             PhysicsComponent::SpeedType::MOVEMENT);
  }
  tangible.physics.updateX(tangible.position);

  if (tangible.collider.horizontalLevelCollision(tangible.position) &&
      !beingHurt) {
    tangible.direction *= -1;
  }

  tangible.physics.updateY(tangible.position);

  if (tangible.collider.verticalLevelCollision(tangible.position)) {
    tangible.physics.setSpdy(0.f, PhysicsComponent::SpeedType::MOVEMENT);
  }
}
} // namespace script