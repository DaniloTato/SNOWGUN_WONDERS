#include "AnimatedObject.hpp"
#include "GeneralContext.hpp"

AnimatedObject::AnimatedObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    Renderizer::registerPair(this, &renderizer, params.registerAsRectShape);
}

void AnimatedObject::update(const GeneralContext& ctx) {
    scripter.runScripts(*this, ctx);
    animator.update();
    renderizer.setRect(animator.getCurrentFrame(), 1);
}