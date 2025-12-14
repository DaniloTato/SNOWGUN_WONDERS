#include "BulletManager.hpp"
#include "BasicCollider.hpp"
#include "SFML/System/Vector2.hpp"

BulletManager& BulletManager::getInstance() {
    static BulletManager instance;
    return instance;
}

void BulletManager::queueSpawn(
    RenderizerParameters& params,
    BulletType type,
    const sf::Vector2f& speed,
    const sf::Vector2f& accel,
    float damageRadius,
    float range
) {
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

Bullet* BulletManager::createFromRequest(const BulletCreationRequest& req) {
    Bullet* bullet = new Bullet(
        req.params,
        req.type,
        req.speed,
        req.accel,
        req.damageRadius,
        req.range
    );

    bullet->animator.loadFromAsepriteJSON("assets/json/bullet.json");
    return bullet;
}

void BulletManager::destroyObject(Bullet* bullet) {
    GameObject::destroy(bullet);
}

void BulletManager::update(const GeneralContext& ctx) {
    for (Bullet* bullet : objects) {
        if (bullet->isDead()) {
            queueDeletion(bullet);
        }
    }
}

Bullet* BulletManager::isCollidingWithBullet(TangibleObject& object) {

    for (Bullet* bullet : objects) {
        if (!bullet) continue;

        if (BasicCollider::objectsColliding(&object, bullet)) {
            return bullet;
        }
    }
    return nullptr;
}