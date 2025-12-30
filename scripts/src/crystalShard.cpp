#include "crystalShard.hpp"

#include "BasicCollider.hpp"
#include "CollectableManager.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "ParticleManager.hpp"
#include "PhysicsComponent.hpp"
#include "SFML/System/Vector2.hpp"

namespace script {

struct CrystalState {
  bool collected = false;
  bool isBig = false;
  sf::Vector2f startingSpeed;

  CrystalState(bool isBig, sf::Vector2f startingSpeed)
      : isBig(isBig), startingSpeed(startingSpeed) {}
};

void crystalShard(TangibleObject &obj, const GeneralContext &ctx) {
  bool crystalSize = (rand() % 3 == 0);
  auto &state = obj.scripter.getState<CrystalState>(
      "crystal", crystalSize,
      sf::Vector2f(Helper::randRange(-4.f, 4.f), Helper::randRange(3.f, 6.f)));

  if (state.collected) {
    if (!obj.isPlayingAnySound()) {
      CollectableManager::getInstance().queueDeleteCollectable(&obj);
    }
    return;
  }

  // set speed first frame
  if (state.startingSpeed != sf::Vector2f(0.f, 0.f)) {
    obj.physics.setSpdx(state.startingSpeed.x,
                        PhysicsComponent::SpeedType::MOVEMENT);
    obj.physics.setSpdy(state.startingSpeed.y,
                        PhysicsComponent::SpeedType::MOVEMENT);

    if (state.isBig) {
      obj.collider.setSize({16.f, 16.f});
      obj.animator.play("big");
    } else {
      obj.collider.setSize({8.f, 8.f});
      obj.animator.play("little");
    }

    state.startingSpeed = {0.f, 0.f};
  }

  obj.physics.updateX(obj.position);
  obj.collider.horizontalLevelCollision(obj.position);

  obj.physics.updateY(obj.position);
  if (obj.collider.verticalLevelCollision(obj.position)) {
    float currentSpeedY =
        obj.physics.getSpdy(PhysicsComponent::SpeedType::MOVEMENT);
    obj.physics.setSpdy(currentSpeedY * -0.95f,
                        PhysicsComponent::SpeedType::MOVEMENT);
  }

  if (BasicCollider::objectsColliding(ctx.player, &obj)) {
    state.collected = true;
    obj.renderizer.hide();
    ParticleManager::getInstance().emitCross(obj.position);
    obj.playSound(rand() % 2 == 0 ? "crystal" : "crystal2");

    GameState::getInstance().addToCrystalAmount(state.isBig ? 2 : 1);
  }
}

} // namespace script