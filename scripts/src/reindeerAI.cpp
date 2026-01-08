#include "reindeerAI.hpp"

#include "BulletManager.hpp"
#include "GameState.hpp"
#include "GeneralContext.hpp"
#include "damageable.hpp"

#include <cmath>
#include <limits>

namespace script {

namespace {

const float FOLLOW_SPEED = 30.f;
const float PLAYER_DISTANCE = 30.f;
const float SHOOT_DISTANCE = 100.f;
const float FIRE_COOLDOWN = 0.8f;
const float PROJECTILE_SPEED = 2.f;

struct ReindeerAIState {
  float fixedY = std::numeric_limits<float>::max();
  float fireCooldown = 0.f;
};
} // namespace

void reindeerAI(TangibleObject &tangible, const GeneralContext &ctx) {

  script::damageable(tangible, ctx, 6, 0.3f, 0.65f);

  if (script::DamageFunctions::isDying(tangible))
    return;

  auto &state = tangible.scripter.getState<ReindeerAIState>("reindeerAI");

  tangible.collider.computeCollisionGrid(tangible.position);

  if (state.fixedY == std::numeric_limits<float>::max()) {
    state.fixedY = tangible.position.y;
  }

  float dt = GameState::getInstance().dt();
  state.fireCooldown -= dt;

  auto *player = ctx.player;
  if (!player)
    return;

  if (script::DamageFunctions::isBeingHurt(tangible)) {
    tangible.physics.setSpdx(0.f, PhysicsComponent::SpeedType::MOVEMENT);
  } else {
    float dx = player->position.x - tangible.position.x;
    float absDx = std::abs(dx);

    tangible.position.y = state.fixedY;
    tangible.direction = (dx >= 0) ? 1 : -1;

    if (absDx > PLAYER_DISTANCE) {
      tangible.physics.setSpdx(dx / FOLLOW_SPEED,
                               PhysicsComponent::SpeedType::MOVEMENT);
    } else {
      tangible.physics.setSpdx(0.f, PhysicsComponent::SpeedType::MOVEMENT);
    }

    if (absDx < SHOOT_DISTANCE && state.fireCooldown <= 0.f) {

      sf::Vector2f distanceVector = player->position - tangible.position;
      float len = std::sqrt(distanceVector.x * distanceVector.x +
                            distanceVector.y * distanceVector.y);

      if (len > 0.f) {
        sf::Vector2f dir = distanceVector / len;

        sf::Vector2f bulletVel = dir * PROJECTILE_SPEED;

        RenderizerParameters bulletParams{
            *GameState::getInstance().getMainWindow(),
            *ctx.bulletTexture,
            {0, 0, 15, 15},
            tangible.position,
            GameState::getInstance().getMainCamera(),
            0.f,
            1.f};

        BulletManager::getInstance().queueSpawn(
            bulletParams, Bullet::Type::Normal, bulletVel, {0, 0}, 3, 300,
            BulletManager::I_AM_NOT_PLAYER);

        state.fireCooldown = FIRE_COOLDOWN;
      }
    }
  }

  tangible.physics.updateX(tangible.position);

  tangible.collider.horizontalLevelCollision(tangible.position);
  tangible.collider.verticalLevelCollision(tangible.position);
}
} // namespace script