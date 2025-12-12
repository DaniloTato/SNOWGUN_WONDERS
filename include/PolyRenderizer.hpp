#pragma once
#include "Renderizer.hpp"
#include "RenderCommand.hpp"
#include <vector>

class PolyRenderizer : public Renderizer {
public:
    PolyRenderizer(const RenderizerParameters& params);

    void updateRenderCommands(std::vector<RenderCommand>& renderCommands);
    virtual void render(GameObject* obj) override;

private:
    std::vector<RenderCommand> renderCommands;
};