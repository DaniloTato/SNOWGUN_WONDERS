#pragma once
#include <unordered_map>
#include <string>
#include <functional>

#include "QueuedManager.hpp"
#include "TangibleObject.hpp"
#include "SFML/Graphics/Texture.hpp"

struct EnemyCreationRequest {
    std::string templateName;
    sf::Vector2f position;
};

class EnemyManager
    : public QueuedManager<TangibleObject, EnemyCreationRequest>
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

    void registerTemplate(
        const std::string& name,
        std::function<TangibleObject*(const sf::Vector2f&)> fn
    );

    sf::Texture& loadTexture(const std::string& name, const std::string& path);
    sf::Texture& getTexture(const std::string& name);

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

protected:
    TangibleObject* createFromRequest(const EnemyCreationRequest& req) override;
    void destroyObject(TangibleObject* obj) override;

private:
    EnemyManager() = default;

    std::unordered_map<std::string,
        std::function<TangibleObject*(const sf::Vector2f&)>> templates;

    std::unordered_map<std::string, sf::Texture> enemyTextures;

    std::vector<EnemySpawnDefinition> spawnDefinitions;
};