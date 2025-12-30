#pragma once
#include <string>

#include "TangibleObject.hpp"
#include "Cache.hpp"
#include "SpawnableManager.hpp"
#include "SFML/Graphics/Texture.hpp"

struct EnemyCreationRequest {
    std::string templateName;
    sf::Vector2f position;
};

class EnemyManager
    : public SpawnableManager<TangibleObject, EnemyCreationRequest>
{
public:

    enum class EnemySpawnTriggerType {
        AREA_ENTER,
        TIMER,
        PROXIMITY,
    };

    struct EnemySpawnDefinition {
        std::string id;
        std::string templateName;
        EnemySpawnTriggerType triggerType;
        sf::FloatRect triggerArea;
        float spawnInterval = 0.f;
        float spawnTimer = 0.f;
        sf::Vector2f spawnPoint;
        int maxAlive = 1;
        float activationRadius = 500.f;
        bool triggeredOnce = false;
        bool enabled = true;
    };

    static EnemyManager& getInstance();

    void queueCreateEnemy(const std::string& templateName,
                          const sf::Vector2f& position);

    void queueDeleteEnemy(TangibleObject* enemy);

    std::vector<std::string> getEnemyList() const;

    TangibleObject* isCollidingWithEnemy(TangibleObject& object);

    void checkSpawnTriggers(const sf::Vector2f& playerPos); //Must be run every frame

    void loadSpawnDefinitionsFromJson(const std::string& path);
    void saveSpawnDefinitionsToJson(const std::string& path);

    void addSpawnDefinition(const EnemySpawnDefinition& def);
    void removeSpawnDefinition(const std::string& id);
    void removeSpawnDefinition( const sf::Vector2f& position,float tolerance);
    std::vector<EnemySpawnDefinition>& getSpawnDefinitions();

    Cache<sf::Texture> textureCache;
    Cache<Animations> animationCache;

protected:
    void destroyObject(TangibleObject* obj) override;

private:
    EnemyManager() = default;

    std::vector<EnemySpawnDefinition> spawnDefinitions;
};