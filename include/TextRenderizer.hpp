#pragma once
#include "Renderizer.hpp"
#include "Glyph.hpp"
#include <vector>

class TextRenderizer : public Renderizer {
public:
    TextRenderizer(const RenderizerParameters& params);

    void updateRenderGlyphs(std::vector<RenderGlyph>& renderGlyphs);
    virtual void render(GameObject* obj) override;

private:
    std::vector<RenderGlyph> renderGlyphs;
};