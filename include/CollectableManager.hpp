#pragma once
#include "QueuedManager.hpp"
#include "TangibleObject.hpp"
#include <unordered_map>
#include <functional>

struct CollectableCreationRequest {
    std::string templateName;
    sf::Vector2f position;
};

class CollectableManager
    : public QueuedManager<TangibleObject, CollectableCreationRequest>
{
public:
    static CollectableManager& getInstance();

    using CollectableFactory =
        std::function<TangibleObject*(const sf::Vector2f&)>;

    void registerTemplate(
        const std::string& name,
        CollectableFactory fn
    );

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
    TangibleObject* createFromRequest(
        const CollectableCreationRequest& req
    ) override;

    void destroyObject(TangibleObject* obj) override;

private:
    std::unordered_map<std::string, CollectableFactory> templates;
    std::unordered_map<std::string, sf::Texture> collectableTextures;
    std::unordered_map<std::string, Animations> collectableAnimations;
};