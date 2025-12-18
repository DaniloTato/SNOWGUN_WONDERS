#include "Bullet.hpp"
#include "Constants.hpp"
#include "PhysicsComponent.hpp"

Bullet::Bullet(
    RenderizerParameters params,
    Bullet::Type t,
    const sf::Vector2f& initSpeed,
    const sf::Vector2f& accel,
    float dmgRadius,
    float range,
    bool shotByPlayer
)
: TangibleObject(params),
  type(t),
  acceleration(accel),
  damageRadius(dmgRadius),
  maxRange(range),
  spawnPos(params.position),
  shotByPlayer(shotByPlayer)
{

    physics.setSpeed(initSpeed, PhysicsComponent::SpeedType::MOVEMENT);
    if(type != Bullet::Type::BubbleGun){
        physics.turnOffFriction();
    }else{
        physics.xFriction = 0.99f;
    }

    switch (type) {
        case Bullet::Type::Normal:
            physics.gravity = 0.f;
            break;

        case Bullet::Type::BubbleGun:
            physics.gravity = 0.3f;
            maxBounces = 10;
            maxLifeTime = 8.f;
            break;

        case Bullet::Type::Bazooka:
            physics.gravity = 0.f;
            break;
    }

    collider.horizontalLevelCollision(position);
    collider.verticalLevelCollision(position);
}

void Bullet::update(const GeneralContext& ctx) {
    if (dead) return;

    if (!dying) {
        updateBehavior(ctx);
        checkLifetime(ctx);
    } else {
        animator.update();
        if (animator.animationFinished()) {
            dead = true;
        }
    }

    TangibleObject::update(ctx);
}

void Bullet::updateBehavior(const GeneralContext& ctx) {
    physics.setSpeed(physics.getSpeed(PhysicsComponent::SpeedType::MOVEMENT) + acceleration, PhysicsComponent::SpeedType::MOVEMENT);

    physics.updateY(position);
    if (collider.verticalLevelCollision(position)) {
        if (type == Bullet::Type::BubbleGun) {
            bounceCount++;
            physics.setSpdy(-physics.getSpdy(PhysicsComponent::SpeedType::MOVEMENT) * 0.9f, PhysicsComponent::SpeedType::MOVEMENT);

            if (bounceCount >= maxBounces)
                die();
        }
        else {
            die();
        }
    }

    physics.updateX(position);
    if (collider.horizontalLevelCollision(position)){
        if (type == Bullet::Type::BubbleGun) {
            bounceCount++;
            physics.setSpdx(-physics.getSpdx(PhysicsComponent::SpeedType::MOVEMENT) * 0.9f, PhysicsComponent::SpeedType::MOVEMENT);

            if (bounceCount >= maxBounces)
                die();
        }
        else {
            die();
        }
    }
}

void Bullet::checkLifetime(const GeneralContext& ctx) {
    lifeTimer += 1.f/Constants::FRAME_RATE;

    if (maxLifeTime > 0.f && lifeTimer >= maxLifeTime)
        die();

    float dist = std::hypot(
        position.x - spawnPos.x,
        position.y - spawnPos.y
    );

    if (dist >= maxRange)
        die();
}

void Bullet::die() {
    if (dying) return;

    dying = true;
    animator.play("die_once");

    collider.setSize({ damageRadius * 2.f, damageRadius * 2.f });
    collider.setOffset({ -damageRadius, -damageRadius });
}

bool Bullet::isShotByPlayer(){
    return shotByPlayer;
}