#pragma once

#include "GameObject.hpp"
#include "Renderizer.hpp"

class RenderableObject: public GameObject {
public:
    RenderableObject(RenderizerParameters params);
    void update(const GeneralContext& ctx) override;
    Renderizer renderizer;
};