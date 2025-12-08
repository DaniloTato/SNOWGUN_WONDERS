#include "TangibleObject.hpp"
#include "InputManager.hpp"

TangibleObject::TangibleObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    GameObject::getGameObjects().push_back(this);
}

void TangibleObject::update() {
    renderizer.render(position);
    collider.calculateCollisionGrid(position);

    if(InputManager::getInstance().isPressed("left")){
        position.x -= 1.f;
    } else if (InputManager::getInstance().isPressed("right")){
        position.x += 1.f;
    }
    collider.horizontalLevelCollision(position);

    if(InputManager::getInstance().isPressed("up")){
        position.y -= 1.f;
    } else if (InputManager::getInstance().isPressed("down")){
        position.y += 1.f;
    }
    collider.verticalLevelCollision(position);
}