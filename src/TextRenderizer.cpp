#include "TextRenderizer.hpp"
#include "Renderizer.hpp"

#include <iostream>

TextRenderizer::TextRenderizer(const RenderizerParameters& params): Renderizer(params){}

void TextRenderizer::updateRenderGlyphs(std::vector<RenderGlyph>& newRenderGlyphs){
    renderGlyphs = newRenderGlyphs;
}

void TextRenderizer::render(GameObject* obj){
    if (!assignedCamera) return;

    for(RenderGlyph glyph: renderGlyphs){
        sprite.setTextureRect(glyph.rect);
        sprite.setColor(glyph.color);
        sf::Vector2f screenPos = assignedCamera->worldToScreen(glyph.pos, paralax);
        sprite.setPosition(screenPos);
        sprite.setScale(assignedCamera->getZoom(), assignedCamera->getZoom());
        window.draw(sprite);
    }
}