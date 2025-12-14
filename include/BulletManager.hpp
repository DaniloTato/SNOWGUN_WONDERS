#pragma once
#include "QueuedManager.hpp"
#include "Bullet.hpp"
#include "GameObject.hpp"
#include "BasicCollider.hpp"

struct BulletCreationRequest {
    RenderizerParameters params;
    BulletType type;
    sf::Vector2f speed;
    sf::Vector2f accel;
    float damageRadius;
    float range;
};

class BulletManager
    : public GameObject
    , public QueuedManager<Bullet, BulletCreationRequest>
{
public:
    static BulletManager& getInstance();

    void queueSpawn(
        RenderizerParameters& params,
        BulletType type,
        const sf::Vector2f& speed,
        const sf::Vector2f& accel,
        float damageRadius,
        float range
    );

    void queueDeletion(Bullet* bullet);

    void update(const GeneralContext& ctx) override;

    Bullet* isCollidingWithBullet(TangibleObject& object);

private:
    BulletManager() = default;

    Bullet* createFromRequest(const BulletCreationRequest& req) override;
    void destroyObject(Bullet* bullet) override;
};