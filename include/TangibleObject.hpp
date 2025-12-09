#pragma once
#include "GeneralContext.hpp"
#include "GameObject.hpp"
#include "Renderizer.hpp"
#include "BasicCollider.hpp"
#include "Scripter.hpp"
#include "Animator.hpp"
#include "PhysicsComponent.hpp"

class TangibleObject: public GameObject {
public:
    TangibleObject(RenderizerParameters params);
    void update(const GeneralContext& ctx) override;
    Renderizer renderizer;
    BasicCollider collider;
    Scripter<TangibleObject> scripter;
    Animator animator;
    PhysicsComponent physics;
    int direction = 1;
};