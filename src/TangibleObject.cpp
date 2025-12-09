#include "TangibleObject.hpp"
#include "GeneralContext.hpp"

TangibleObject::TangibleObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    GameObject::getGameObjects().push_back(this);
}

void TangibleObject::update(const GeneralContext& ctx) {
    renderizer.render(position);
    collider.calculateCollisionGrid(position);
    scripter.runScripts(*this, ctx);
}