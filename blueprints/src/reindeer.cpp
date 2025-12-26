#include "reindeer.hpp"

#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "reindeerAI.hpp"
#include "Helpers.hpp"

std::filesystem::path ROOT;

namespace blueprint {

    TangibleObject* reindeer(const sf::Vector2f& pos) {

        ROOT = Helper::getExecutableDir();

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

        TangibleObject* heli = new TangibleObject(params);

        heli->collider.setOffset({4.f, 4.f});
        heli->collider.setSize({24.f, 8.f});

        heli->animator.loadAsepriteAnimations((ROOT / "assets\\json\\reindeer.json").string());

        heli->animator.play("fly");

        heli->scripter.addScript(script::reindeerAI);

        return heli;
    }

}