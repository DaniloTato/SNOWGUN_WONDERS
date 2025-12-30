#include "healthBlueprint.hpp"

#include "GameState.hpp"
#include "CollectableManager.hpp"
#include "healthPickup.hpp"
#include "Helpers.hpp"

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

        auto* hp = new TangibleObject(params,
            Animator::getAsepriteJSONAnimations(Helper::getPath("assets/json/ginger.json")));

        hp->collider.setSize({16.f, 16.f});
        hp->animator.play("idle");

        hp->scripter.addScript(script::healthPickup);

        return hp;
    }

}