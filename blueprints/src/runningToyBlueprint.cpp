#include "runningToy.hpp"

#include "EnemyManager.hpp"
#include "GameState.hpp"

namespace blueprint {

    TangibleObject* runningToyBlueprint(const sf::Vector2f& pos) {

        const std::string id = "toy";

        RenderizerParameters params{
            *GameState::getInstance().getMainWindow(),
            EnemyManager::getInstance().getTexture(id),
            {0,0,17,17},
            pos,
            GameState::getInstance().getMainCamera(),
            0.f,
            1.f
        };

        TangibleObject* toy = new TangibleObject(params, 
            EnemyManager::getInstance().getCachedAnimations(id));

        toy->collider.setOffset({5.f, 2.f});
        toy->collider.setSize({15.f, 14.f});

        toy->animator.play("walking");

        toy->scripter.addScript(script::runningToy);

        toy->makesDamageTroughContact = false;

        return toy;
    }

}