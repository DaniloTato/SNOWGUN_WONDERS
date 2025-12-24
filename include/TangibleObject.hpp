#pragma once
#include "GameObject.hpp"
#include "Renderizer.hpp"
#include "BasicCollider.hpp"
#include "Scripter.hpp"
#include "Animator.hpp"
#include "PhysicsComponent.hpp"

struct AttackHitbox {
    BasicCollider collider;
    bool active = false;
    int damage = 1;
    float remainingTime = 0.f;
};

class TangibleObject: public GameObject {
public:
    TangibleObject(RenderizerParameters params);
    void update(const GeneralContext& ctx) override;
    Renderizer renderizer;
    BasicCollider collider;
    Scripter<TangibleObject> scripter;
    Animator animator;
    PhysicsComponent physics;


    std::optional<AttackHitbox> attackHitbox;
    int direction = 1;
    bool makesDamageTroughContact = true; //Horrible, but whatever. I'll change it eventually.
};