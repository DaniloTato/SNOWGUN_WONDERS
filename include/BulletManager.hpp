#pragma once
#include <vector>
#include "Bullet.hpp"
#include "GameObject.hpp"

struct BulletCreationRequest {
    RenderizerParameters params;
    BulletType type;
    sf::Vector2f speed;
    sf::Vector2f accel;
    float damageRadius;
    float range;
};

class BulletManager: GameObject {
public:
    static BulletManager& getInstance();

    void spawn(
        RenderizerParameters& params,
        BulletType type,
        const sf::Vector2f& speed,
        const sf::Vector2f& accel,
        float damageRadius,
        float range
    );

    void queueSpawn(
        RenderizerParameters& params,
        BulletType type,
        const sf::Vector2f& speed,
        const sf::Vector2f& accel,
        float damageRadius,
        float range
    );

    void queueDeletion(Bullet*);

    virtual void update(const GeneralContext& ctx) override;
    static void applyQueuedBulletChanges();
    void deleteBullet(Bullet* bullet);
    //definitely will need to implement a queueing object

private:
    std::vector<BulletCreationRequest> createQueue;
    std::vector<Bullet*> deleteQueue;
    BulletManager() = default;
    std::vector<Bullet*> bullets;
};