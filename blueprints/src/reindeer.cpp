#include "reindeer.hpp"

#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "reindeerAI.hpp"

namespace blueprint {

    TangibleObject* reindeer(const sf::Vector2f& pos) {

        const std::string id = "reindeer";

        RenderizerParameters params{
            *GameState::getInstance().getMainWindow(),
            EnemyManager::getInstance().getTexture(id),
            {0,0,32,16},
            pos,
            GameState::getInstance().getMainCamera(),
            0.f,
            1.f
        };

        TangibleObject* heli = new TangibleObject(params, 
            EnemyManager::getInstance().getCachedAnimations(id));

        heli->collider.setOffset({4.f, 4.f});
        heli->collider.setSize({24.f, 8.f});

        heli->animator.play("fly");

        heli->scripter.addScript(script::reindeerAI);

        return heli;
    }

}