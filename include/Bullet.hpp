#pragma once
#include "TangibleObject.hpp"

enum class BulletType {
    Normal,
    BubbleGun,
    Bazooka
};

class Bullet : public TangibleObject {
public:
    Bullet(
        RenderizerParameters params,
        BulletType type,
        const sf::Vector2f& initSpeed,
        const sf::Vector2f& acceleration,
        float damageRadius,
        float maxRange
    );

    void update(const GeneralContext& ctx) override;

    bool isDead() const { return dead; }
    void die();

private:
    void updateBehavior(const GeneralContext& ctx);
    void checkLifetime(const GeneralContext& ctx);

private:
    BulletType type;

    sf::Vector2f acceleration;
    float damageRadius;
    float maxRange;

    sf::Vector2f spawnPos;

    int bounceCount = 0;
    int maxBounces = 0;

    float lifeTimer = 0.f;
    float maxLifeTime = 0.f;

    bool dying = false;
    bool dead = false;
};