#include "healthBlueprint.hpp"

#include "GameState.hpp"
#include "CollectableManager.hpp"
#include "healthPickup.hpp"   // script

namespace blueprint {

    TangibleObject* healthBlueprint(const sf::Vector2f& pos) {

        const std::string id = "health";

        RenderizerParameters params{
            *GameState::getInstance().getMainWindow(),
            CollectableManager::getInstance().getTexture(id),
            {0, 0, 16, 16},
            pos,
            GameState::getInstance().getMainCamera(),
            0.f,
            1.f
        };

        auto* hp = new TangibleObject(params);

        hp->collider.setSize({12.f, 12.f});
        hp->collider.setOffset({2.f, 2.f});

        //hp->physics.enableGravity = true;

        hp->scripter.addScript(script::healthPickup);

        return hp;
    }

}