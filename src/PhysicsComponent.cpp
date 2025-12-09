#include "PhysicsComponent.hpp"

PhysicsComponent::PhysicsComponent() 
    : speed(0.f, 0.f), gravity(0.3f), xFriction(0.9f) {}

PhysicsComponent::PhysicsComponent(float gravityValue)
    : speed(0.f, 0.f), gravity(gravityValue) {}

void PhysicsComponent::updateX(sf::Vector2f& position) {
    speed.x *= xFriction;
    position.x += speed.x;
}

void PhysicsComponent::updateY(sf::Vector2f& position) {
    speed.y += gravity;
    position.y += speed.y;
}