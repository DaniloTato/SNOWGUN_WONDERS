#pragma once
#include "QueuedManager.hpp"
#include "Bullet.hpp"
#include "GameObject.hpp"
#include "BasicCollider.hpp"

struct BulletCreationRequest {
    RenderizerParameters params;
    Bullet::Type type;
    sf::Vector2f speed;
    sf::Vector2f accel;
    float damageRadius;
    float range;
    bool shotByPlayer;
};

class BulletManager
    : public GameObject
    , public QueuedManager<Bullet, BulletCreationRequest>
{
public:

    inline static const bool I_AM_PLAYER = true;
    inline static const bool I_AM_NOT_PLAYER = false;

    static BulletManager& getInstance();

    void queueSpawn(
        RenderizerParameters& params,
        Bullet::Type type,
        const sf::Vector2f& speed,
        const sf::Vector2f& accel,
        float damageRadius,
        float range,
        bool shotByPlayer
    );

    void queueDeletion(Bullet* bullet);

    void update(const GeneralContext& ctx) override;

    Bullet* isCollidingWithBullet(TangibleObject& object, bool amIPlayer);

private:
    BulletManager() = default;

    Bullet* createFromRequest(const BulletCreationRequest& req) override;
    void destroyObject(Bullet* bullet) override;
};