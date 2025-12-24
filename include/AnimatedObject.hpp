#pragma once

#include "Renderizer.hpp"
#include "Animator.hpp"

class AnimatedObject: public GameObject {
public:
    AnimatedObject(RenderizerParameters params);
    void update(const GeneralContext& ctx) override;
    Renderizer renderizer;
    Scripter<AnimatedObject> scripter;
    Animator animator;
};