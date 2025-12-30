#pragma once
#include "SpawnableManager.hpp"
#include "TangibleObject.hpp"
#include <unordered_map>

struct CollectableCreationRequest {
    std::string templateName;
    sf::Vector2f position;
};

class CollectableManager
    : public SpawnableManager<TangibleObject, CollectableCreationRequest>
{
public:
    static CollectableManager& getInstance();

    void queueCreateCollectable(
        const std::string& templateName,
        const sf::Vector2f& position
    );

    void queueDeleteCollectable(TangibleObject* obj);

    std::vector<std::string> getCollectableList() const;

    sf::Texture& loadTexture(const std::string& name, const std::string& path);
    sf::Texture& getTexture(const std::string& name);

    void cacheAnimations(const std::string& collectableId, std::string animationPath);
    Animations& getCachedAnimations(const std::string& name);

protected:

    void destroyObject(TangibleObject* obj) override;

private:
    std::unordered_map<std::string, sf::Texture> collectableTextures;
    std::unordered_map<std::string, Animations> collectableAnimations;
};