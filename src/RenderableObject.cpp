#include "RenderableObject.hpp"
#include "GeneralContext.hpp"

RenderableObject::RenderableObject(RenderizerParameters params) : GameObject(params.position), renderizer(params) {
    Renderizer::registerPair(this, &renderizer);
}

void RenderableObject::update(const GeneralContext& ctx) {

}