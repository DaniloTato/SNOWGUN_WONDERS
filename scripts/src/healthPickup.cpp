#include "healthPickup.hpp"
#include "CollectableManager.hpp"
#include "GameState.hpp"
#include "ParticleManager.hpp"

namespace script {

namespace {
struct HealthPickupState {
  int heal = 1;
  bool collected = false;
};
} // namespace

void healthPickup(TangibleObject &obj, const GeneralContext &ctx) {
  auto &state = obj.scripter.getState<HealthPickupState>("health");

  if (state.collected) {
    if (!obj.isPlayingAnySound()) {
      CollectableManager::getInstance().queueDeleteCollectable(&obj);
    }
    return;
  }

  if (BasicCollider::objectsColliding(ctx.player, &obj)) {
    state.collected = true;
    obj.renderizer.hide();
    obj.playSound("health", 150);
    ParticleManager::getInstance().emitHearts(obj.position, 5);
    GameState::getInstance().changePlayerHealth(1);
  }
}

} // namespace script