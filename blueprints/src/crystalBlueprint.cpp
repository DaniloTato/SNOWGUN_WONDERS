#include "crystalBlueprint.hpp"

#include "GameState.hpp"
#include "CollectableManager.hpp"
#include "crystalShard.hpp"
#include "Helpers.hpp"

const std::filesystem::path ROOT = Helper::getExecutableDir();

namespace blueprint {

    TangibleObject* crystalBlueprint(const sf::Vector2f& pos) {

        const std::string id = "crystal";

        RenderizerParameters params{
            *GameState::getInstance().getMainWindow(),
            CollectableManager::getInstance().getTexture(id),
            {0, 0, 16, 16},
            pos,
            GameState::getInstance().getMainCamera(),
            0.f,
            1.f
        };

        auto* crystal = new TangibleObject(params);

        crystal->collider.setSize({12.f, 12.f});
        crystal->collider.setOffset({2.f, 2.f});

        // crystal->physics.enableGravity = true;
        // crystal->physics.mass = 1.f;

        crystal->animator.loadAsepriteAnimations(ROOT / "assets/json/crystal.json");
        crystal->animator.play("big");

        crystal->scripter.addScript(script::crystalShard);

        return crystal;
    }
}