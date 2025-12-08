#include "TangibleObject.hpp"

TangibleObject::TangibleObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    GameObject::getGameObjects().push_back(this);
}

void TangibleObject::update() {
    renderizer.render(position);
    collider.calculateCollisionGrid(position);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        position.x -= 1.f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        position.x += 1.f;
    }
    collider.horizontalLevelCollision(position);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        position.y -= 1.f;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        position.y += 1.f;
    }
    collider.verticalLevelCollision(position);
}