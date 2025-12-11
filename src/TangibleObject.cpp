#include "TangibleObject.hpp"
#include "GeneralContext.hpp"
#include "Constants.hpp"

TangibleObject::TangibleObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    Renderizer::registerPair(this, &renderizer);
}

void TangibleObject::update(const GeneralContext& ctx) {
    collider.calculateCollisionGrid(position);
    scripter.runScripts(*this, ctx);
    animator.update(1.f / Constants::FRAME_RATE);
    renderizer.setRect(animator.getCurrentFrame(), direction);
}