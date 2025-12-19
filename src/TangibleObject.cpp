#include "TangibleObject.hpp"
#include "GeneralContext.hpp"

TangibleObject::TangibleObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    Renderizer::registerPair(this, &renderizer, params.registerAsRectShape);
}

void TangibleObject::update(const GeneralContext& ctx) {
    collider.computeCollisionGrid(position);
    scripter.runScripts(*this, ctx);
    animator.update();
    renderizer.setRect(animator.getCurrentFrame(), direction);
}