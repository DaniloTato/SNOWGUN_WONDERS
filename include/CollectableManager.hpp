#pragma once
#include "SpawnableManager.hpp"
#include "TangibleObject.hpp"
#include "Cache.hpp"

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

    Cache<sf::Texture> textureCache;
    Cache<Animations> animationCache;

protected:

    void destroyObject(TangibleObject* obj) override;
};