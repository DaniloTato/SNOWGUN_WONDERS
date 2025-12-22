#pragma once
#include "TangibleObject.hpp"

class Collectable : public TangibleObject {
public:
    Collectable(RenderizerParameters params)
        : TangibleObject(params) {}

    bool collected = false;
};