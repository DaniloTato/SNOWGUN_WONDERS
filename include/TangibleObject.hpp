#pragma once
#include "ContextTypes.hpp"
#include "GameObject.hpp"
#include "Renderizer.hpp"
#include "BasicCollider.hpp"
#include "Scripter.hpp"

class TangibleObject: public GameObject {
public:
    TangibleObject(RenderizerParameters params);
    void update(const GameContext& ctx) override;
    Renderizer renderizer;
    BasicCollider collider;
    Scripter<TangibleObject, TangibleContext> scripter;
};