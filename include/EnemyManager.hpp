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

protected:
    TangibleObject* createFromRequest(const EnemyCreationRequest& req) override;
    void destroyObject(TangibleObject* obj) override;

private:
    EnemyManager() = default;

    std::unordered_map<std::string,
        std::function<TangibleObject*(const sf::Vector2f&)>> templates;

    std::unordered_map<std::string, sf::Texture> enemyTextures;
};