#include "EnemyManager.hpp"
#include "GameObject.hpp"
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