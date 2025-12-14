#include "Bullet.hpp"
#include "Constants.hpp"

Bullet::Bullet(
    RenderizerParameters params,
    BulletType t,
    const sf::Vector2f& initSpeed,
    const sf::Vector2f& accel,
    float dmgRadius,
    float range
)
: TangibleObject(params),
  type(t),
  acceleration(accel),
  damageRadius(dmgRadius),
  maxRange(range),
  spawnPos(params.position)
{

    physics.setSpeed(initSpeed);
    if(type != BulletType::BubbleGun){
        physics.turnOffFriction();
    }else{
        physics.setXFriction(0.99f);
    }
    animator.setState("fly");

    switch (type) {
        case BulletType::Normal:
            physics.setGravity(0.f);
            break;

        case BulletType::BubbleGun:
            physics.setGravity(0.3f);
            maxBounces = 10;
            maxLifeTime = 8.f;
            break;

        case BulletType::Bazooka:
            physics.setGravity(0.f);
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
    physics.setSpeed(physics.getSpeed() + acceleration);

    physics.updateY(position);
    if (collider.verticalLevelCollision(position)) {
        if (type == BulletType::BubbleGun) {
            bounceCount++;
            physics.setSpdy(-physics.getSpdy() * 0.9f);

            if (bounceCount >= maxBounces)
                die();
        }
        else {
            die();
        }
    }

    physics.updateX(position);
    if (collider.horizontalLevelCollision(position)){
        if (type == BulletType::BubbleGun) {
            bounceCount++;
            physics.setSpdx(-physics.getSpdx() * 0.9f);

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
    animator.setState("die_once");

    collider.setSize({ damageRadius * 2.f, damageRadius * 2.f });
    collider.setOffset({ -damageRadius, -damageRadius });
}