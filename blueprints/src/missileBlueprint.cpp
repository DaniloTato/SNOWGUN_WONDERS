#include "missileBlueprint.hpp"

#include "EnemyManager.hpp"
#include "GameState.hpp"
#include "Helpers.hpp"
#include "missileAI.hpp"

const std::filesystem::path ROOT = Helper::getExecutableDir();

namespace blueprint {

    TangibleObject* missileBlueprint(const sf::Vector2f& pos) {

        const std::string id = "missile";

        RenderizerParameters params{
            *GameState::getInstance().getMainWindow(),
            EnemyManager::getInstance().getTexture(id),
            {0,0,48,48},
            pos,
            GameState::getInstance().getMainCamera(),
            -11.f,
            1.f
        };

        TangibleObject* missile = new TangibleObject(params);

        missile->collider.setOffset({0.f, 16.f});
        missile->collider.setSize({32.f, 32.f});

        missile->animator.loadAsepriteAnimations(ROOT / "assets/json/missile.json");

        missile->animator.play("idle");

        missile->scripter.addScript(script::missileAI);
        missile->direction = -1;

        return missile;
    }

}