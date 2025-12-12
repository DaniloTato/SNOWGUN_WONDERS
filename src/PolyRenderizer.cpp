#include "PolyRenderizer.hpp"
#include "Renderizer.hpp"

PolyRenderizer::PolyRenderizer(const RenderizerParameters& params): Renderizer(params){}

void PolyRenderizer::updateRenderCommands(std::vector<RenderCommand>& newRenderCommands){
    renderCommands = newRenderCommands;
    //copy is created, but i dont want to get into dynamic memory for this one
}

void PolyRenderizer::render(GameObject* obj){
    if (!assignedCamera) return;

    for(const RenderCommand& command: renderCommands){
        sprite.setTextureRect(command.rect);
        sprite.setColor(command.color);
        sf::Vector2f screenPos = assignedCamera->worldToScreen(command.pos, paralax);
        sprite.setPosition(screenPos);
        sprite.setScale(assignedCamera->getZoom(), assignedCamera->getZoom());
        window.draw(sprite);
    }
}