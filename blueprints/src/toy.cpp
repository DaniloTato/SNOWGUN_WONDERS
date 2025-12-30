#include "toy.hpp"

#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "enemyPatrol.hpp"

namespace blueprint {

    TangibleObject* toy(const sf::Vector2f& pos) {

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

        toy->scripter.addScript(script::enemyPatrol);
        toy->direction = -1;

        return toy;
    }

}