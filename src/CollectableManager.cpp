#include "CollectableManager.hpp"
#include "GameObject.hpp"
#include <iostream>

//Posibility of unifting Collectablemanager and EnemyManager using inherency?

CollectableManager& CollectableManager::getInstance() {
    static CollectableManager instance;
    return instance;
}

void CollectableManager::queueCreateCollectable(
    const std::string& templateName,
    const sf::Vector2f& position
) {
    createQueue.push_back({templateName, position});
}

void CollectableManager::queueDeleteCollectable(
    TangibleObject* obj
) {
    deleteQueue.push_back(obj);
}

void CollectableManager::destroyObject(TangibleObject* obj) {
    GameObject::destroy(obj);
}

std::vector<std::string> CollectableManager::getCollectableList() const {
    std::vector<std::string> result;
    result.reserve(templates.size());

    for (const auto& pair : templates)
        result.push_back(pair.first);

    return result;
}

sf::Texture& CollectableManager::loadTexture(const std::string& name, const std::string& path){
    auto& tex = collectableTextures[name];
    if (!tex.loadFromFile(path)) {
        std::cerr << "[EnemyManager] Failed to load texture: " << path << "\n";
    }
    return tex;
}

sf::Texture& CollectableManager::getTexture(const std::string& name){
    return collectableTextures.at(name);
}

void CollectableManager::cacheAnimations(const std::string& collectableId, std::string animationPath) {
    collectableAnimations[collectableId] = Animator::getAsepriteJSONAnimations(animationPath);
}

Animations& CollectableManager::getCachedAnimations(const std::string& name) {
    return collectableAnimations.at(name);
}