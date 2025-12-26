#include "BulletManager.hpp"
#include "BasicCollider.hpp"
#include "SFML/System/Vector2.hpp"
#include "Helpers.hpp"

const std::filesystem::path ROOT = Helper::getExecutableDir();

BulletManager& BulletManager::getInstance() {
    static BulletManager instance;
    return instance;
}

void BulletManager::queueSpawn(
    RenderizerParameters& params,
    Bullet::Type type,
    const sf::Vector2f& speed,
    const sf::Vector2f& accel,
    float damageRadius,
    float range,
    bool shotByPlayer
) {
    createQueue.push_back({
        params,
        type,
        speed,
        accel,
        damageRadius,
        range,
        shotByPlayer
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
        req.range,
        req.shotByPlayer
    );

    bullet->animator.loadAsepriteAnimations((ROOT / "assets\\json\\bullet.json").string());
    bullet->animator.play("fly");
    return bullet;
}

void BulletManager::destroyObject(Bullet* bullet) {
    GameObject::destroy(bullet);
}

void BulletManager::update() {
    for (Bullet* bullet : objects) {
        if (bullet->isDead()) {
            queueDeletion(bullet);
        }
    }
}

Bullet* BulletManager::isCollidingWithBullet(TangibleObject& object, bool amIPlayer) {

    for (Bullet* bullet : objects) {

        if (!bullet->isDying() && 
        BasicCollider::objectsColliding(&object, bullet) &&
            ((amIPlayer && !bullet->isShotByPlayer()) || (!amIPlayer && bullet->isShotByPlayer()))
        ) {
            return bullet;
        }
    }
    return nullptr;
}