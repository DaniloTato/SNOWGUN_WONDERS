#include "RenderableObject.hpp"

RenderableObject::RenderableObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    GameObject::getGameObjects().push_back(this);
}

void RenderableObject::update() {
    renderizer.render(position);
}