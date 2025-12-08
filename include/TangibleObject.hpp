#pragma once
#include "GameObject.hpp"
#include "Renderizer.hpp"
#include "BasicCollider.hpp"

class TangibleObject: public GameObject {
public:
    TangibleObject(RenderizerParameters params);
    void update() override;
    Renderizer renderizer;
    BasicCollider collider;
};