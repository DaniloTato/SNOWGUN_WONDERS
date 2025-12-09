#include "TangibleObject.hpp"

TangibleObject::TangibleObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    GameObject::getGameObjects().push_back(this);
}

void TangibleObject::update(const GameContext& ctx) {
    renderizer.render(position);
    collider.calculateCollisionGrid(position);
    
    if (std::holds_alternative<TangibleContext>(ctx)) {
        const TangibleContext& tangibleCtx = std::get<TangibleContext>(ctx);
        scripter.runScripts(*this, tangibleCtx);
    }
}