#include "crystalShard.hpp"

#include "BasicCollider.hpp"
#include "CollectableManager.hpp"
#include "ParticleManager.hpp"
#include "PhysicsComponent.hpp"
#include "SFML/System/Vector2.hpp"

namespace script {

    struct CrystalState {
        bool collected = false;
        bool isBig = false;
        sf::Vector2f startingSpeed;

        CrystalState(bool isBig, sf::Vector2f startingSpeed): isBig(isBig), startingSpeed(startingSpeed){}
    };

    void crystalShard(TangibleObject& obj, const GeneralContext& ctx) {
        bool crystalSize = (rand() % 3 == 0);
        auto& state = obj.scripter.getState<CrystalState>("crystal", crystalSize, sf::Vector2f(rand() % 8 - 4, 3 + rand() % 3));

        if (state.collected) return;

        //set speed first frame
        if(state.startingSpeed != sf::Vector2f(0.f,0.f)){
            obj.physics.setSpdx(state.startingSpeed.x, PhysicsComponent::SpeedType::MOVEMENT);
            obj.physics.setSpdy(state.startingSpeed.y, PhysicsComponent::SpeedType::MOVEMENT);

            if(state.isBig){
                obj.collider.setSize({16.f,16.f});
                obj.animator.play("big");
            } else{
                obj.collider.setSize({8.f,8.f});
                obj.animator.play("little");
            }

            state.startingSpeed = {0.f,0.f};
        }

        obj.collider.computeCollisionGrid(obj.position);

        obj.physics.updateX(obj.position);
        obj.collider.horizontalLevelCollision(obj.position);

        obj.physics.updateY(obj.position);
        if(obj.collider.verticalLevelCollision(obj.position)){
            float currentSpeedY = obj.physics.getSpdy(PhysicsComponent::SpeedType::MOVEMENT);
            obj.physics.setSpdy(currentSpeedY * -0.95, PhysicsComponent::SpeedType::MOVEMENT);
        }

        if (BasicCollider::objectsColliding(ctx.player, &obj)) {
            state.collected = true;
            ParticleManager::getInstance().emitCross(obj.position);

            //ctx.player->addCrystals(1);

            CollectableManager::getInstance().queueDeleteCollectable(&obj);
        }
    }

}