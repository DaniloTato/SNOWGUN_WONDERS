#include "EnemyManager.hpp"
#include "GameObject.hpp"
#include "damageable.hpp"
#include <iostream>

EnemyManager& EnemyManager::getInstance() {
    static EnemyManager instance;
    return instance;
}

void EnemyManager::registerTemplate(
    const std::string& name,
    std::function<TangibleObject*(const sf::Vector2f&)> fn
) {
    templates[name] = fn;
}

sf::Texture& EnemyManager::loadTexture(
    const std::string& name,
    const std::string& path
) {
    auto& tex = enemyTextures[name];
    if (!tex.loadFromFile(path)) {
        std::cerr << "[EnemyManager] Failed to load texture: " << path << "\n";
    }
    return tex;
}

sf::Texture& EnemyManager::getTexture(const std::string& name) {
    return enemyTextures.at(name);
}

void EnemyManager::queueCreateEnemy(
    const std::string& templateName,
    const sf::Vector2f& position
) {
    createQueue.push_back({templateName, position});
}

void EnemyManager::queueDeleteEnemy(TangibleObject* enemy) {
    deleteQueue.push_back(enemy);
}

TangibleObject* EnemyManager::createFromRequest(
    const EnemyCreationRequest& req
) {
    auto it = templates.find(req.templateName);
    if (it == templates.end()) {
        std::cerr << "[EnemyManager] Unknown enemy template: "
                  << req.templateName << "\n";
        return nullptr;
    }

    return it->second(req.position);
}

void EnemyManager::destroyObject(TangibleObject* obj) {
    GameObject::destroy(obj);
}

std::vector<std::string> EnemyManager::getEnemyList() const {
    std::vector<std::string> result;
    result.reserve(templates.size());

    for (const auto& pair : templates) {
        result.push_back(pair.first);
    }

    return result;
}

TangibleObject* EnemyManager::isCollidingWithEnemy(TangibleObject& object) {
    for (TangibleObject* enemy : objects) {

        if (!enemy) continue;

        if (enemy->attackHitbox.has_value()) {
            if(BasicCollider::isCollidingRect(
                enemy->attackHitbox.value().collider.getCollisionRect(enemy->position),
                object.collider.getCollisionRect(object.position))
            )
            {
                return enemy;
            }
        }

        if (script::DamageFunctions::isDying(*enemy)) continue;

        if (enemy->makesDamageTroughContact && BasicCollider::objectsColliding(&object, enemy)) {
            return enemy;
        }
    }

    return nullptr;
}