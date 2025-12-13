#include "BulletManager.hpp"

BulletManager& BulletManager::getInstance() {
    static BulletManager instance;
    return instance;
}

void BulletManager::spawn(
    RenderizerParameters& params,
    BulletType type,
    const sf::Vector2f& speed,
    const sf::Vector2f& accel,
    float damageRadius,
    float range
) {
    Bullet* bullet = new Bullet(
        params,
        type,
        speed,
        accel,
        damageRadius,
        range
    );

    bullet -> animator.loadFromAsepriteJSON("assets/json/bullet.json");
    bullet -> animator.setSpeedMultiplier(1.5f);

    getInstance().bullets.push_back(bullet);
}

void BulletManager::queueSpawn(
    RenderizerParameters& params,
    BulletType type,
    const sf::Vector2f& speed,
    const sf::Vector2f& accel,
    float damageRadius,
    float range
){
    createQueue.push_back({
        params,
        type,
        speed,
        accel,
        damageRadius,
        range
    });
}

void BulletManager::queueDeletion(Bullet* bullet) {
    deleteQueue.push_back(bullet);
}

void BulletManager::update(const GeneralContext& ctx) {

    for(Bullet* bullet: bullets){
        if(bullet -> isDead()){
            queueDeletion(bullet);
        }
    }
}

void BulletManager::deleteBullet(Bullet* bullet) {
    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),
            [&](Bullet* b) {
                if (b == bullet) {
                    GameObject::destroy(b);
                    return true;
                }
                return false;
            }
        ),
        bullets.end()
    );
}

void BulletManager::applyQueuedBulletChanges() {
    BulletManager& mgr = getInstance();

    for (auto& req : mgr.deleteQueue) {
        mgr.deleteBullet(req);
    }
    mgr.deleteQueue.clear();

    for (auto& req : mgr.createQueue) {
        mgr.spawn(
            req.params,
            req.type,
            req.speed,
            req.accel,
            req.damageRadius,
            req.range
        );
    }
    mgr.createQueue.clear();
}