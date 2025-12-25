#include "BasicCollider.hpp"
#include "BulletManager.hpp"
#include "CollectableManager.hpp"
#include "chest.hpp"

namespace script {

    namespace{
        struct ChestState {
            bool opened = false;
        };
    }

    void chest(TangibleObject& chest, const GeneralContext& ctx) {
        auto& state = chest.scripter.getState<ChestState>("chest");

        if (state.opened) return;


        if (BulletManager::getInstance().isCollidingWithBullet(chest, false)) {

            state.opened = true;

            if(rand() % 2 == 0){
                CollectableManager::getInstance().queueCreateCollectable("health", {chest.position.x, chest.position.y - 10});
            }

            int numberOfCrystals = (rand() % 10 + 20);

            for(int i = 0; i < numberOfCrystals; i++){
                CollectableManager::getInstance().queueCreateCollectable("crystal", chest.position + sf::Vector2f(16.f,0.f));
            }

            chest.animator.play("open_once");
            chest.playSound("chest");
        }
    }

}