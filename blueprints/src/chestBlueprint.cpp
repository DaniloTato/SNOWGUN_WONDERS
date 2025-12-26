#include "chest.hpp"

#include "GameState.hpp"
#include "CollectableManager.hpp"
#include "chest.hpp"
#include <filesystem>
#include "Helpers.hpp"

const std::filesystem::path ROOT = Helper::getExecutableDir();

namespace blueprint {

    TangibleObject* chestBlueprint(const sf::Vector2f& pos) {

        const std::string id = "chest";

        RenderizerParameters params{
            *GameState::getInstance().getMainWindow(),
            CollectableManager::getInstance().getTexture(id),
            {0, 0, 16, 16},
            pos,
            GameState::getInstance().getMainCamera(),
            0.f,
            1.f
        };

        auto* chest = new TangibleObject(params);

        chest->collider.setSize({14.f, 10.f});
        chest->collider.setOffset({1.f, 6.f});

        chest->animator.loadAsepriteAnimations((ROOT / "assets\\json\\chest.json").string());
        chest->animator.play("closed");

        chest->scripter.addScript(script::chest);

        return chest;
    }

}